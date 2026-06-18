#ifndef UI_H
#define UI_H

void ui_show_menu(void);
void ui_get_number(double *num);
void ui_get_input(double *a, double *b);
void ui_print_result(double result);
void ui_get_expression(char *buf, size_t size);
void ui_print_error(const char *msg);
void ui_clear(void);

#endif
