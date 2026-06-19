/*
 * parser.c - 递归下降解析器实现
 *
 * 解析器采用经典的三层结构处理运算符优先级：
 *
 *   parse_expr   → 加法/减法（最低优先级）
 *     └─ parse_term   → 乘法/除法（较高优先级）
 *          └─ parse_factor → 最基本单元：数字、括号、函数调用
 *
 * 支持的语法：
 *   数字：42, 3.14, -5
 *   运算符：+ - * /
 *   括号：(1+2)*3
 *   函数：sqrt(x), pow(a,b), sin(x), cos(x), tan(x)
 *
 * Parser 结构体保存解析状态：
 *   pos   - 当前扫描位置（指针不断前进）
 *   error - 错误标志（1=出错）
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "parser.h"

typedef struct {
    const char *pos;  /* 当前扫描到字符串的哪个位置，解析过程中不断前进 */
    int error;        /* 错误标志：0=正常，1=解析出错（语法错误、除零等） */
} Parser;

static double parse_expr(Parser *p);

/*
 * 匹配关键字（如 "sqrt"、"sin"）
 * 逻辑：比较字符串前缀，同时确保关键字后面不是字母
 *       防止 "sincos" 被误匹配成 "sin" + 残余 "cos"
 * 返回：1=匹配成功（指针前进），0=不匹配
 */
static int match_keyword(Parser *p, const char *kw)
{
    size_t len = strlen(kw);
    if (strncmp(p->pos, kw, len) == 0 && !isalpha((unsigned char)p->pos[len])) {
        p->pos += len;  /* 匹配成功，指针前进 */
        return 1;
    }
    return 0;
}

/* 跳过空格和制表符，让解析器忽略用户输入中的空白 */
static void skip_whitespace(Parser *p)
{
    while (*p->pos == ' ' || *p->pos == '\t')
        p->pos++;
}

/*
 * 解析数字（整数或浮点数）
 * 使用标准库 strtod：把字符串转成 double
 * end 指向转换结束位置，如果 end == p->pos 说明开头不是数字
 */
static double parse_number(Parser *p)
{
    skip_whitespace(p);
    char *end;
    double value = strtod(p->pos, &end);  /* 尝试转换数字 */
    if (end == p->pos) {
        p->error = 1;  /* 开头不是数字，标记错误 */
        return 0;
    }
    p->pos = end;  /* 指针前进到数字之后 */
    return value;
}

static double parse_factor(Parser *p);

/*
 * 解析最基本的语法单元（优先级最高）
 *
 * 支持的语法：
 *   1. 一元正负号：+5, -3
 *   2. 括号表达式：(1+2)
 *   3. 内置函数：sqrt(x), pow(a,b), sin(x), cos(x), tan(x)
 *   4. 数字：42, 3.14
 *
 * 递归调用处理嵌套：如 "-(1+2)" 会递归调用 parse_factor 处理 "-"
 */
static double parse_factor(Parser *p)
{
    skip_whitespace(p);

    /* 一元正号：+expr 等价于 expr */
    if (*p->pos == '+') {
        p->pos++;
        return parse_factor(p);  /* 递归，处理如 "+-5" 这种连续符号 */
    }

    /* 一元负号：-expr 取反 */
    if (*p->pos == '-') {
        p->pos++;
        return -parse_factor(p);  /* 递归，处理如 "--5" 这种连续符号 */
    }

    /* 括号表达式：(expr) 递归调用 parse_expr 解析括号内容 */
    if (*p->pos == '(') {
        p->pos++;
        double value = parse_expr(p);  /* 递归解析括号内的表达式 */
        skip_whitespace(p);
        if (*p->pos != ')') {
            p->error = 1;  /* 缺少右括号 */
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
        if (*p->pos != '(') { p->error = 1; return 0; }  /* 期望左括号 */
        p->pos++;
        double val = parse_expr(p);  /* 递归解析参数（支持嵌套表达式） */
        skip_whitespace(p);
        if (*p->pos != ')') { p->error = 1; return 0; }  /* 期望右括号 */
        p->pos++;
        if (val < 0.0) { p->error = 1; return 0; }  /* 负数不能开方 */
        return sqrt(val);
    }

    if (match_keyword(p, "pow")) {
        skip_whitespace(p);
        if (*p->pos != '(') { p->error = 1; return 0; }
        p->pos++;
        double base = parse_expr(p);  /* 第一个参数 */
        skip_whitespace(p);
        if (*p->pos != ',') { p->error = 1; return 0; }  /* 期望逗号分隔 */
        p->pos++;
        double exp = parse_expr(p);  /* 第二个参数 */
        skip_whitespace(p);
        if (*p->pos != ')') { p->error = 1; return 0; }
        p->pos++;
        return pow(base, exp);
    }

    /*
     * 三角函数支持 (参数为弧度):
     *   sin(x)  - 正弦
     *   cos(x)  - 余弦
     *   tan(x)  - 正切
     *
     * 使用示例:
     *   sin(0)             -> 0
     *   cos(0)             -> 1
     *   sin(3.14159/6)     -> 0.5
     *   cos(3.14159/3)     -> 0.5
     *   tan(3.14159/4)     -> 1
     *   sqrt(sin(3.14159/2) + 1) -> sqrt(2) ≈ 1.41421
     */
    if (match_keyword(p, "sin")) {
        skip_whitespace(p);
        if (*p->pos != '(') { p->error = 1; return 0; }
        p->pos++;
        double val = parse_expr(p);
        skip_whitespace(p);
        if (*p->pos != ')') { p->error = 1; return 0; }
        p->pos++;
        return sin(val);
    }

    if (match_keyword(p, "cos")) {
        skip_whitespace(p);
        if (*p->pos != '(') { p->error = 1; return 0; }
        p->pos++;
        double val = parse_expr(p);
        skip_whitespace(p);
        if (*p->pos != ')') { p->error = 1; return 0; }
        p->pos++;
        return cos(val);
    }

    if (match_keyword(p, "tan")) {
        skip_whitespace(p);
        if (*p->pos != '(') { p->error = 1; return 0; }
        p->pos++;
        double val = parse_expr(p);
        skip_whitespace(p);
        if (*p->pos != ')') { p->error = 1; return 0; }
        p->pos++;
        return tan(val);
    }

    /* 最后尝试解析数字 */
    return parse_number(p);
}

/*
 * 解析乘除项（优先级高于加减）
 *
 * 语法：factor ((*|/) factor)*
 * 用 while 循环支持连续乘除：2*3*4 会依次计算
 * 除法时检查除零
 */
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
                p->error = 1;  /* 除零错误 */
                return 0;
            }
            value /= right;
        }
        skip_whitespace(p);
    }

    return value;
}

/*
 * 解析加减表达式（优先级最低）
 *
 * 语法：term ((+|-) term)*
 * 这是递归下降的最顶层，被 parse_expression 调用
 */
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

/*
 * 对外接口：解析表达式并返回结果
 *
 * 流程：
 *   1. 初始化 Parser，pos 指向字符串开头
 *   2. 调用 parse_expr 开始递归解析
 *   3. 跳过尾部空白
 *   4. 检查：有错误 或 字符串没解析完 → 返回 -1
 *
 * 为什么要检查 *p.pos != '\0'？
 *   防止用户输入 "1+2abc" 时，解析器算出 3 就返回了，
 *   剩余的 "abc" 被忽略，掩盖输入错误
 */
int parse_expression(const char *expr, double *result)
{
    Parser p = { expr, 0 };  /* 初始化：pos 指向开头，error=0 */
    *result = parse_expr(&p);
    skip_whitespace(&p);

    if (p.error || *p.pos != '\0')
        return -1;  /* 语法错误 */

    return 0;  /* 成功 */
}
