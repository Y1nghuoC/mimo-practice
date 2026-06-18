#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

void ui_show_menu(void)
{
    printf("\n========== 命令行简易计算器 ==========\n");
    printf("  [1] 加法    [2] 减法\n");
    printf("  [3] 乘法    [4] 除法\n");
    printf("  [5] 清零    [6] 表达式\n");
    printf("  [7] 开方    [8] 幂运算\n");
    printf("  [9] sin     [10] cos\n");
    printf("  [11] tan    [0] 退出\n");
    printf("======================================\n");
    printf("请选择操作: ");
}

void ui_get_number(double *num)
{
    while (scanf("%lf", num) != 1) {
        while (getchar() != '\n')
            ;
        printf("输入无效，请输入一个数字: ");
    }
}

void ui_get_input(double *a, double *b)
{
    printf("请输入第一个数: ");
    ui_get_number(a);
    printf("请输入第二个数: ");
    ui_get_number(b);
}

void ui_get_expression(char *buf, size_t size)
{
    printf("请输入表达式 (如 (1+2)*3): ");
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';
}

void ui_print_result(double result)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%.15f", result);
    char *dot = strchr(buf, '.');
    if (dot) {
        char *end = buf + strlen(buf) - 1;
        while (end > dot && *end == '0')
            *end-- = '\0';
        if (end == dot)
            *end = '\0';
    }
    printf("计算结果: %s\n", buf);
}

void ui_print_error(const char *msg)
{
    printf("错误: %s\n", msg);
}

void ui_clear(void)
{
    system("cls");
    printf("已清零。\n");
}
