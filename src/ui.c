/*
 * ui.c - 用户界面模块的实现
 *
 * 所有函数都只处理 I/O，不包含计算逻辑。
 * 输入验证采用循环重试模式：直到用户输入合法数据才返回。
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

/* 打印主菜单，显示 12 个选项（1-11 功能 + 0 退出） */
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

/*
 * 读取一个数字
 * 验证逻辑：
 *   scanf 返回 1 → 读取成功，退出循环
 *   scanf 返回 0 → 输入不是数字，清空缓冲区，提示重试
 *
 * 内层 while(getchar()!='\n') 的作用：
 *   清空 stdin 缓冲区里的残留数据（如用户输入了 "abc"），
 *   否则下次 scanf 还会读到同样的脏数据，陷入死循环
 */
void ui_get_number(double *num)
{
    while (scanf("%lf", num) != 1) {
        while (getchar() != '\n')  /* 清空缓冲区 */
            ;
        printf("输入无效，请输入一个数字: ");
    }
}

/* 依次读取两个数字，用于二元运算（加/减/乘/除/幂） */
void ui_get_input(double *a, double *b)
{
    printf("请输入第一个数: ");
    ui_get_number(a);
    printf("请输入第二个数: ");
    ui_get_number(b);
}

/*
 * 读取表达式字符串
 * fgets 会把行尾的 '\n' 也读进来
 * strcspn 找到 '\n' 的位置，手动替换成 '\0' 截断
 */
void ui_get_expression(char *buf, size_t size)
{
    printf("请输入表达式 (如 (1+2)*3): ");
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';  /* 去掉末尾换行符 */
}

/*
 * 打印计算结果，自动格式化
 * 逻辑：
 *   1. 先用 "%.15f" 格式化为 15 位小数（保证精度）
 *   2. 从尾部去掉多余的 '0'（如 3.140000 → 3.14）
 *   3. 如果小数部分全是 0，连小数点也去掉（如 3.000000 → 3）
 */
void ui_print_result(double result)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%.15f", result);
    char *dot = strchr(buf, '.');
    if (dot) {
        char *end = buf + strlen(buf) - 1;
        while (end > dot && *end == '0')
            *end-- = '\0';  /* 去掉尾部的 0 */
        if (end == dot)
            *end = '\0';  /* 小数部分全是 0，连点也去掉 */
    }
    printf("计算结果: %s\n", buf);
}

/* 打印错误消息，格式：错误: <消息> */
void ui_print_error(const char *msg)
{
    printf("错误: %s\n", msg);
}

/*
 * 清屏（Windows 专用）
 * system("cls") 调用 Windows 命令行清屏命令
 * 注意：Linux/macOS 应使用 "clear"
 */
void ui_clear(void)
{
    system("cls");
    printf("已清零。\n");
}
