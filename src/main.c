#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include "parser.h"
#include "ui.h"

int main(void)
{
    system("chcp 65001 >nul");

    int choice;
    double a, b, result;
    char expr[256];

    while (1) {
        ui_show_menu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n')
                ;
            ui_print_error("无效输入，请输入数字");
            continue;
        }
        while (getchar() != '\n')
            ;

        if (choice == 0) {
            printf("退出程序，再见！\n");
            break;
        }

        if (choice == 5) {
            ui_clear();
            continue;
        }

        if (choice == 6) {
            ui_get_expression(expr, sizeof(expr));
            if (parse_expression(expr, &result) == 0)
                ui_print_result(result);
            else
                ui_print_error("表达式语法错误");
            continue;
        }

        if (choice == 7) {
            printf("请输入要开方的数: ");
            ui_get_number(&a);
            if (calc_sqrt(a, &result) == 0)
                ui_print_result(result);
            else
                ui_print_error("不能对负数开方");
            continue;
        }

        if (choice == 8) {
            ui_get_input(&a, &b);
            calc_pow(a, b, &result);
            ui_print_result(result);
            continue;
        }

        if (choice < 1 || choice > 4) {
            ui_print_error("无效选项，请重新选择");
            continue;
        }

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
                ui_print_result(result);
            } else {
                ui_print_error("除数不能为零");
            }
            break;
        }
    }

    return 0;
}
