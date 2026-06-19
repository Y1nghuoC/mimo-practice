/*
 * calc.c - 数学运算模块的实现
 *
 * 所有函数都是纯计算逻辑，不涉及 I/O。
 * 错误处理采用"返回状态码 + 指针输出结果"模式：
 *   - 返回 0 表示成功，-1 表示失败
 *   - 失败时不写入 *result，调用方应检查返回值
 */
#include <math.h>
#include "calc.h"

/* 加法：a + b，不可能失败 */
double calc_add(double a, double b)
{
    return a + b;
}

/* 减法：a - b，不可能失败 */
double calc_sub(double a, double b)
{
    return a - b;
}

/* 乘法：a * b，不可能失败 */
double calc_mul(double a, double b)
{
    return a * b;
}

/*
 * 除法：a / b
 * 防御逻辑：如果 b 为 0，拒绝计算并返回 -1
 * 为什么用指针输出：因为失败时无结果可返回，不能用 return result
 */
int calc_div(double a, double b, double *result)
{
    if (b == 0.0) {
        return -1;  /* 除数为零，拒绝计算 */
    }
    *result = a / b;
    return 0;  /* 成功 */
}

/*
 * 开方：sqrt(x)
 * 防御逻辑：负数不能开方，返回 -1
 */
int calc_sqrt(double x, double *result)
{
    if (x < 0.0) {
        return -1;  /* 负数不能开方 */
    }
    *result = sqrt(x);
    return 0;  /* 成功 */
}

/* 幂运算：pow(base, exp)，当前版本不会出错 */
int calc_pow(double base, double exp, double *result)
{
    *result = pow(base, exp);
    return 0;
}

/* 正弦函数：参数为弧度 */
double calc_sin(double x)
{
    return sin(x);
}

/* 余弦函数：参数为弧度 */
double calc_cos(double x)
{
    return cos(x);
}

/* 正切函数：参数为弧度 */
double calc_tan(double x)
{
    return tan(x);
}
