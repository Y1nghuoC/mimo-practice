/*
 * ui.h - 用户界面模块
 *
 * 职责：封装所有用户交互（打印菜单、读取输入、输出结果、报错）
 * 设计：main.c 不直接 printf/scanf，全部通过本模块的函数
 * 好处：将来换成 GUI 或添加日志时，只需改 ui.c
 */
#ifndef UI_H
#define UI_H

/* 打印主菜单，显示所有可用操作 */
void ui_show_menu(void);

/* 读取一个数字，输入无效时循环提示重新输入 */
void ui_get_number(double *num);

/* 读取两个数字（用于二元运算：加/减/乘/除） */
void ui_get_input(double *a, double *b);

/* 打印计算结果，自动格式化（去掉末尾多余的零） */
void ui_print_result(double result);

/* 读取表达式字符串（用于表达式模式） */
void ui_get_expression(char *buf, size_t size);

/* 打印错误消息 */
void ui_print_error(const char *msg);

/* 清屏（Windows 专用：调用 cls 命令） */
void ui_clear(void);

#endif
