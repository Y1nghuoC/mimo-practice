/*
 * parser.h - 表达式解析模块
 *
 * 职责：将用户输入的字符串表达式（如 "(1+2)*3"）解析为数值结果
 *
 * 对外只暴露一个函数 parse_expression，内部使用递归下降解析器。
 *
 * 返回值约定：
 *   0  = 解析成功，结果存入 *result
 *  -1  = 表达式语法错误
 */
#ifndef PARSER_H
#define PARSER_H

/*
 * 解析并计算表达式
 * 参数：
 *   expr   - 待解析的表达式字符串（如 "2*(3+4)"）
 *   result - 输出参数，解析成功时存入计算结果
 * 返回值：0=成功，-1=语法错误
 */
int parse_expression(const char *expr, double *result);

#endif
