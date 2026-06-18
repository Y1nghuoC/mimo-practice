#include <math.h>
#include "calc.h"

double calc_add(double a, double b)
{
    return a + b;
}

double calc_sub(double a, double b)
{
    return a - b;
}

double calc_mul(double a, double b)
{
    return a * b;
}

int calc_div(double a, double b, double *result)
{
    if (b == 0.0) {
        return -1;
    }
    *result = a / b;
    return 0;
}

int calc_sqrt(double x, double *result)
{
    if (x < 0.0) {
        return -1;
    }
    *result = sqrt(x);
    return 0;
}

int calc_pow(double base, double exp, double *result)
{
    *result = pow(base, exp);
    return 0;
}
