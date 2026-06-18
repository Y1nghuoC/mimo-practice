#ifndef CALC_H
#define CALC_H

double calc_add(double a, double b);
double calc_sub(double a, double b);
double calc_mul(double a, double b);
int    calc_div(double a, double b, double *result);
int    calc_sqrt(double x, double *result);
int    calc_pow(double base, double exp, double *result);
double calc_sin(double x);
double calc_cos(double x);
double calc_tan(double x);

#endif
