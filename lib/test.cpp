#include <stdio.h>

#include "binopts.cpp"

int main() {
    unsigned int x = 40;
    unsigned int y = 5;

    printf("Addition:       %d + %d = %d\n", x, y, x+y);
    printf("Subtraction:    %d - %d = %d\n", x, y, x-y);
    printf("Multiplication: %d * %d = %d\n", x, y, x*y);
    printf("Division:       %d / %d = %d\n", x, y, x/y);

    float a = 40.0;
    float b = 5.0;

    printf("Addition:       %f + %f = %f\n", a, b, rpiLop_fpadd(a,b));
    printf("Subtraction:    %f - %f = %f\n", a, b, rpiLop_fpsub(a,b));
    printf("Multiplication: %f * %f = %f\n", a, b, rpiLop_fpmul(a,b));
    printf("Division:       %f / %f = %f\n", a, b, rpiLop_fpdiv(a,b));
}
