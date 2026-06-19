/*
 * calc.h - 数学运算模块
 *
 * 职责：封装所有基本数学运算（加减乘除、开方、幂运算、三角函数）
 *
 * 设计约定：
 * - 不可能出错的运算（加/减/乘/三角函数）直接返回 double
 * - 可能出错的运算（除法/开方）返回 int 状态码（0=成功，-1=失败），
 *   结果通过指针参数传出
 */
#ifndef CALC_H
#define CALC_H

/* 基础运算：直接返回结果，不会出错 */
double calc_add(double a, double b);  /* 返回 a + b */
double calc_sub(double a, double b);  /* 返回 a - b */
double calc_mul(double a, double b);  /* 返回 a * b */

/*
 * 除法运算：带除零保护
 * 返回值：0=成功，-1=除数为零
 * 结果存入 *result
 */
int calc_div(double a, double b, double *result);

/*
 * 开方运算：带负数检查
 * 返回值：0=成功，-1=输入为负数
 * 结果存入 *result
 */
int calc_sqrt(double x, double *result);

/*
 * 幂运算：计算 base 的 exp 次方
 * 返回值：0=成功（当前版本不会失败）
 * 结果存入 *result
 */
int calc_pow(double base, double exp, double *result);

/* 三角函数：参数为弧度，直接返回结果 */
double calc_sin(double x);  /* 返回 sin(x) */
double calc_cos(double x);  /* 返回 cos(x) */
double calc_tan(double x);  /* 返回 tan(x) */

#endif
