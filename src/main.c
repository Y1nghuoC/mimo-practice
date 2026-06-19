/*
 * main.c - 程序入口和主循环
 *
 * 职责：读取用户菜单选择 → 调用对应模块 → 显示结果
 * 本文件不包含计算逻辑，只做调度分发
 *
 * 架构：
 *   main（调度层）
 *     ├── ui（I/O 层）
 *     ├── calc（运算层）
 *     └── parser（解析层）
 */
#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include "parser.h"
#include "ui.h"

int main(void)
{
    /* 设置控制台为 UTF-8 编码，让中文正常显示（Windows 专用） */
    system("chcp 65001 >nul");

    int choice;
    double a, b, result;
    char expr[256];

    while (1) {
        ui_show_menu();

        /* 读取菜单选择 */
        if (scanf("%d", &choice) != 1) {
            /* 输入不是数字，清空缓冲区并提示错误 */
            while (getchar() != '\n')
                ;
            ui_print_error("无效输入，请输入数字");
            continue;
        }
        /*
         * 清空输入缓冲区
         * scanf("%d") 只读走数字，留下 '\n' 在缓冲区
         * 这行代码把 '\n' 及后续残留全部清掉，
         * 防止影响后面的 fgets 或 ui_get_number
         */
        while (getchar() != '\n')
            ;

        /* 选项 0：退出程序 */
        if (choice == 0) {
            printf("退出程序，再见！\n");
            break;
        }

        /* 选项 5：清屏 */
        if (choice == 5) {
            ui_clear();
            continue;
        }

        /* 选项 6：表达式模式 */
        if (choice == 6) {
            ui_get_expression(expr, sizeof(expr));
            if (parse_expression(expr, &result) == 0)
                ui_print_result(result);      /* 解析成功，显示结果 */
            else
                ui_print_error("表达式语法错误");  /* 解析失败，报错 */
            continue;
        }

        /* 选项 7：开方 */
        if (choice == 7) {
            printf("请输入要开方的数: ");
            ui_get_number(&a);
            if (calc_sqrt(a, &result) == 0)
                ui_print_result(result);      /* 开方成功 */
            else
                ui_print_error("不能对负数开方");  /* 负数开方失败 */
            continue;
        }

        /* 选项 8：幂运算 */
        if (choice == 8) {
            ui_get_input(&a, &b);
            calc_pow(a, b, &result);
            ui_print_result(result);
            continue;
        }

        /* 选项 9/10/11：三角函数 */
        if (choice == 9 || choice == 10 || choice == 11) {
            printf("请输入角度 (弧度): ");
            ui_get_number(&a);
            if (choice == 9)
                result = calc_sin(a);
            else if (choice == 10)
                result = calc_cos(a);
            else
                result = calc_tan(a);
            ui_print_result(result);
            continue;
        }

        /* 非法选项检查 */
        if (choice < 1 || choice > 4) {
            ui_print_error("无效选项，请重新选择");
            continue;
        }

        /* 选项 1-4：四则运算 */
        ui_get_input(&a, &b);

        switch (choice) {
        case 1:
            result = calc_add(a, b);
            ui_print_result(result);
            break;
        case 2:
            result = calc_sub(a, b);
            ui_print_result(result);
            break;
        case 3:
            result = calc_mul(a, b);
            ui_print_result(result);
            break;
        case 4:
            if (calc_div(a, b, &result) == 0) {
                ui_print_result(result);      /* 除法成功 */
            } else {
                ui_print_error("除数不能为零");  /* 除零失败 */
            }
            break;
        }
    }

    return 0;
}
