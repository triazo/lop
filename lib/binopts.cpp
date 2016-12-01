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
