#include <stdint.h>

#define exponent(x) ((x << 1) >> 24)
#define mantissa(x) ((x << 9) >> 9)
#define sign(x) (x >> 31)

#define min_float 0x00000000
#define max_float 0xffffffff

int rpiLop_add(int a, int b) {
    while (b!=0) {
        int carry = (a & b);
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}


int rpiLop_subtract(int a, int b)
{
  b = -b;
  while (b!=0)
    {
      int carry = (a & b);
      a = a ^ b;
      b = carry << 1;
    }
  return a;

}
int rpiLop_multiply(int a, int b)
{
  int total = 0;
  for(;b;total+=rpiLop_add(total,a) ,b--);
  return total;
}

unsigned int rpiLop_divide(unsigned int dividend, unsigned int divisor)
{
  unsigned int quotient,remainder,t;
  int bits_left = 8 * sizeof(unsigned int);
  quotient = dividend;
  remainder = 0;
  //for(;bits_left;t=quotient,quotient = rpiLop_add(quotient,quotient); remainder = rpiLop_add(rpiLop_add(remainder,remainder),(quotient<t)),((remainder >= divisor)?remainder = rpiLop_subtract(rem,divisor),quotient= rpiLop_add(quotient,1):quotient=quotient),bits_left--);
  for(t=quotient;bits_left;t=quotient,bits_left--)
    {
      quotient = rpiLop_add(quotient,quotient); //add
      remainder = rpiLop_add(rpiLop_add(remainder,remainder),(quotient<t));//nested add
      (remainder >= divisor)?remainder = rpiLop_subtract(remainder,divisor),quotient= rpiLop_add(quotient,1):quotient=quotient;//conditional subtract and add
    }


  return quotient;

}


typedef union fp_bit_twiddler {
    float f;
    uint32_t i;
} UNSAFEFPTHING;


uint32_t rpiLop_fpadd2(uint32_t x, uint32_t y){
    uint32_t result_mantissa;
    uint32_t result_exponent;
    uint32_t result_sign;

    uint32_t different_sign = sign(x) ^ sign(y);

    // catch NaN
    if (!(exponent(x) ^ 0xFF) && mantissa(x)) return x;
    if (!(exponent(y) ^ 0xFF) && mantissa(y)) return y;

    // catch Inf
    if (!(exponent(x) ^ 0xFF) && !(exponent(y) ^ 0xFF)) {
        // both are inf
        if (different_sign)
            // Inf - Inf
            return 0x7F800000 + 1; // NaN
        else
            // both Inf or -Inf
            return x;
    }
    else if (!(exponent(x) ^ 0xFF)) return x;
    else if (!(exponent(y) ^ 0xFF)) return y;

    // both numbers are non-special
    uint32_t exp_difference;
    if (different_sign) {
        exp_difference = exponent(y) + exponent(x);
    }
    else {
        // no need to account for constant BO
        // beware of underflow
        if (exponent(x) > exponent(y)) exp_difference = exponent(x) - exponent(y);
        else exp_difference = exponent(y) - exponent(x);
    }


    bool x_bigger_abs;
    if      (exponent(x) > exponent(y)) x_bigger_abs = true;
    else if (exponent(x) < exponent(y)) x_bigger_abs = false;
    else if (mantissa(x) > mantissa(y)) x_bigger_abs = true;
    else                                x_bigger_abs = false;

    if (!different_sign) {
        //both numbers have same sign (this is an add)
        result_sign = sign(x);

        if (x_bigger_abs) {
            result_mantissa = (mantissa(x) << 1) + (mantissa(y) << 1) >> exp_difference;
            result_exponent = exponent(x);
        }
        else {
            result_mantissa = (mantissa(y) << 1) + ((mantissa(x) << 1) >> exp_difference);
            result_exponent = exponent(y);
        }
        if (result_mantissa << 31) result_mantissa = (result_mantissa >> 1) + 1;
        else result_mantissa = (result_mantissa >> 1);
    }
    else {
        // this actually is a subtraction

        if (x_bigger_abs) {
            result_sign = sign(x);
            result_exponent = exponent(x);

            // subtract and round to 23 bit 
            // this means making room in our 32bit representation
            result_mantissa = (mantissa(x) << 1) - ((mantissa(y) << 1) >> exp_difference );
        }
        else {
            result_sign = sign(y);
            result_exponent = exponent(y);

            // subtract and round to 23 bit 
            // this means making room in our 32bit representation
            result_mantissa = (mantissa(y) << 1) - ((mantissa(x) << 1) >> exp_difference);
        }

        if (result_mantissa << 31)  result_mantissa = ((result_mantissa >> 1) + 1);
        else result_mantissa = (result_mantissa >> 1);

        // normalize mantissa
        uint32_t temp = result_mantissa << 9;
        for (uint32_t count = 0; count < 23; ++count) {
            if (!((temp << count) >> 31)) {
                result_mantissa <<= ++count; // leading 1, so shift 1 more time
                result_exponent -= count;
                break;
            }
        }
    }
    uint32_t result = result_sign << 31 | result_exponent << 23 | result_mantissa;
    return result;
}

float rpiLop_fpadd(float x, float y){
  UNSAFEFPTHING a, b, result;
  a.f = x;
  b.f = y;

  result.i = rpiLop_fpadd2(a.i, b.i);
  return result.f;
}

float rpiLop_fpsub(float x, float y){
  UNSAFEFPTHING a, b, result;
  a.f = x;
  b.f = y*-1;

  result.i = rpiLop_fpadd2(a.i, b.i);
  return result.f;
}

uint32_t rpiLop_fpmul2(uint32_t x, uint32_t y){
    uint32_t result_mantissa;
    uint32_t result_exponent;
    uint32_t result_sign;

    uint32_t different_sign = sign(x) ^ sign(y);

    // catch NaN
    if (!(exponent(x) ^ 0xFF) && mantissa(x)) return x;
    if (!(exponent(y) ^ 0xFF) && mantissa(y)) return y;

    // catch Inf
    if (!(exponent(x) ^ 0xFF)) return x;
    if (!(exponent(y) ^ 0xFF)) return y;

    result_sign = sign(x) ^ sign(y);
    result_exponent = exponent(x) + exponent(y) - 127; // subtract bias;
    result_mantissa = mantissa(x)*mantissa(y);

    //round;
    if (result_mantissa << 31)  result_mantissa = ((result_mantissa >> 1) + 1);
    else result_mantissa = (result_mantissa >> 1);

    // normalize mantissa
    uint32_t temp = result_mantissa << 9;
    for (uint32_t count = 0; count < 23; ++count) {
        if (!((temp << count) >> 31)) {
            result_mantissa <<= ++count; // leading 1, so shift 1 more time
            result_exponent -= count;
            break;
        }
    }
    
    uint32_t result = result_sign << 31 | result_exponent << 23 | result_mantissa;
    return result;
}



uint32_t rpiLop_fpdiv2(uint32_t x, uint32_t y){
    uint32_t result_mantissa;
    uint32_t result_exponent;
    uint32_t result_sign;

    uint32_t different_sign = sign(x) ^ sign(y);

    // catch NaN
    if (!(exponent(x) ^ 0xFF) && mantissa(x)) return x;
    if (!(exponent(y) ^ 0xFF) && mantissa(y)) return y;

    // catch Inf
    if (!(exponent(x) ^ 0xFF)) return x;
    if (!(exponent(y) ^ 0xFF)) return y;

    result_sign = sign(x) ^ sign(y);
    result_exponent = exponent(x) - exponent(y) + 127; // add bias;
    result_mantissa = mantissa(x)/mantissa(y);

    //round;
    if (result_mantissa << 31)  result_mantissa = ((result_mantissa >> 1) + 1);
    else result_mantissa = (result_mantissa >> 1);

    // normalize mantissa
    uint32_t temp = result_mantissa << 9;
    for (uint32_t count = 0; count < 23; ++count) {
        if (!((temp << count) >> 31)) {
            result_mantissa <<= ++count; // leading 1, so shift 1 more time
            result_exponent -= count;
            break;
        }
    }
    
    uint32_t result = result_sign << 31 | result_exponent << 23 | result_mantissa;
    return result;
}

float rpiLop_fpmul(float x, float y){
  UNSAFEFPTHING a, b, result;
  a.f = x;
  b.f = y;

  result.i = rpiLop_fpmul2(a.i, b.i);
  return result.f; 
}

float rpiLop_fpdiv(float x, float y){
  UNSAFEFPTHING a, b, result;
  a.f = x;
  b.f = y;

  result.i = rpiLop_fpdiv2(a.i, b.i);
  return result.f; 
}
