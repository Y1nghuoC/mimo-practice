#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "parser.h"

typedef struct {
    const char *pos;
    int error;
} Parser;

static double parse_expr(Parser *p);

/* 匹配关键字，成功则移动指针并返回1，否则返回0 */
static int match_keyword(Parser *p, const char *kw)
{
    size_t len = strlen(kw);
    if (strncmp(p->pos, kw, len) == 0 && !isalpha((unsigned char)p->pos[len])) {
        p->pos += len;
        return 1;
    }
    return 0;
}

static void skip_whitespace(Parser *p)
{
    while (*p->pos == ' ' || *p->pos == '\t')
        p->pos++;
}

static double parse_number(Parser *p)
{
    skip_whitespace(p);
    char *end;
    double value = strtod(p->pos, &end);
    if (end == p->pos) {
        p->error = 1;
        return 0;
    }
    p->pos = end;
    return value;
}

static double parse_factor(Parser *p);

static double parse_factor(Parser *p)
{
    skip_whitespace(p);

    /* 一元正号 */
    if (*p->pos == '+') {
        p->pos++;
        return parse_factor(p);
    }

    /* 一元负号 */
    if (*p->pos == '-') {
        p->pos++;
        return -parse_factor(p);
    }

    /* 括号表达式 */
    if (*p->pos == '(') {
        p->pos++;
        double value = parse_expr(p);
        skip_whitespace(p);
        if (*p->pos != ')') {
            p->error = 1;
            return 0;
        }
        p->pos++;
        return value;
    }

    /*
     * 内置函数支持:
     *   sqrt(x)     - 开方，x不能为负数
     *   pow(a, b)   - 幂运算，计算 a 的 b 次方
     *
     * 使用示例:
     *   sqrt(16)        -> 4
     *   sqrt(2*(3+5))   -> 4
     *   pow(2, 10)      -> 1024
     *   pow(sqrt(9), 2) -> 9
     *   (sqrt(9)+1)*2   -> 8
     */
    if (match_keyword(p, "sqrt")) {
        skip_whitespace(p);
        if (*p->pos != '(') { p->error = 1; return 0; }
        p->pos++;
        double val = parse_expr(p);
        skip_whitespace(p);
        if (*p->pos != ')') { p->error = 1; return 0; }
        p->pos++;
        if (val < 0.0) { p->error = 1; return 0; }
        return sqrt(val);
    }

    if (match_keyword(p, "pow")) {
        skip_whitespace(p);
        if (*p->pos != '(') { p->error = 1; return 0; }
        p->pos++;
        double base = parse_expr(p);
        skip_whitespace(p);
        if (*p->pos != ',') { p->error = 1; return 0; }
        p->pos++;
        double exp = parse_expr(p);
        skip_whitespace(p);
        if (*p->pos != ')') { p->error = 1; return 0; }
        p->pos++;
        return pow(base, exp);
    }

    /* 数字 */
    return parse_number(p);
}

static double parse_term(Parser *p)
{
    double value = parse_factor(p);
    skip_whitespace(p);

    while (*p->pos == '*' || *p->pos == '/') {
        char op = *p->pos++;
        double right = parse_factor(p);
        if (op == '*')
            value *= right;
        else {
            if (right == 0.0) {
                p->error = 1;
                return 0;
            }
            value /= right;
        }
        skip_whitespace(p);
    }

    return value;
}

static double parse_expr(Parser *p)
{
    double value = parse_term(p);
    skip_whitespace(p);

    while (*p->pos == '+' || *p->pos == '-') {
        char op = *p->pos++;
        double right = parse_term(p);
        if (op == '+')
            value += right;
        else
            value -= right;
        skip_whitespace(p);
    }

    return value;
}

int parse_expression(const char *expr, double *result)
{
    Parser p = { expr, 0 };
    *result = parse_expr(&p);
    skip_whitespace(&p);

    if (p.error || *p.pos != '\0')
        return -1;

    return 0;
}
