# 命令行简易计算器 — 项目规格说明

## 项目概述

一个基于 C 语言的命令行交互式计算器，支持基础四则运算、表达式计算（含括号和函数），纯标准库实现，无第三方依赖。

## 技术栈

| 项目 | 说明 |
|------|------|
| 语言 | C99（`-std=c99`） |
| 编译器 | GCC（MinGW） |
| 标准库 | `stdio.h` `stdlib.h` `string.h` `ctype.h` `math.h` |
| 构建工具 | Makefile / 手动 gcc 编译 |
| 平台 | Windows（PowerShell），通过 `chcp 65001` 切换 UTF-8 编码 |

## 目录结构

```
命令行简易计算器/
├── src/
│   ├── main.c        # 程序入口，主循环，菜单分发
│   ├── calc.h        # 计算逻辑头文件（含防重复包含宏 CALC_H）
│   ├── calc.c        # 基础运算实现（加减乘除、开方、幂运算、三角函数）
│   ├── ui.h          # 交互界面头文件（含防重复包含宏 UI_H）
│   ├── ui.c          # 菜单显示、用户输入、结果输出、清屏
│   ├── parser.h      # 表达式解析器头文件（含防重复包含宏 PARSER_H）
│   └── parser.c      # 递归下降表达式解析器（支持函数调用）
├── Makefile          # 编译脚本
├── .gitignore        # Git 忽略规则
├── PROJECT_SPEC.md   # 本文件
├── TODO.md           # 待办与已知问题
└── 记录.txt
```

## 编译方式

```bash
# 手动编译
gcc -Wall -Wextra -std=c99 -o calculator.exe src/main.c src/calc.c src/ui.c src/parser.c -lm

# 或使用 Makefile（需安装 make）
make
make clean
```

## 代码规范

1. **头文件防重复包含**：所有 `.h` 文件必须包含 `#ifndef` / `#define` / `#endif` 宏保护
2. **函数命名**：
   - `calc_*` — 计算逻辑函数（`calc_add`, `calc_div`, `calc_sqrt` 等）
   - `ui_*` — 界面交互函数（`ui_show_menu`, `ui_get_input` 等）
   - `parse_*` — 表达式解析函数
3. **错误处理**：返回 `int` 错误码（0 成功，-1 失败），结果通过指针参数返回
4. **数据类型**：统一使用 `double`，同时支持整数和小数
5. **注释规范**：关键逻辑加注释，函数接口在头文件中声明
6. **不引入第三方库**：仅使用 C 标准库 + `math.h`

## 模块职责

### main.c — 程序入口
- 初始化 UTF-8 编码（`chcp 65001`）
- 主循环：显示菜单 → 获取选择 → 分发处理
- 菜单选项：1-加法 2-减法 3-乘法 4-除法 5-清零 6-表达式 7-开方 8-幂运算 9-sin 10-cos 11-tan 0-退出

### calc.h / calc.c — 计算逻辑
| 函数 | 说明 | 参数 |
|------|------|------|
| `calc_add(a, b)` | 加法 | 两操作数 |
| `calc_sub(a, b)` | 减法 | 两操作数 |
| `calc_mul(a, b)` | 乘法 | 两操作数 |
| `calc_div(a, b, *result)` | 除法 | 除数为零返回 -1 |
| `calc_sqrt(x, *result)` | 开方 | 负数返回 -1 |
| `calc_pow(base, exp, *result)` | 幂运算 | 始终返回 0 |
| `calc_sin(x)` | 正弦函数 | 参数为弧度 |
| `calc_cos(x)` | 余弦函数 | 参数为弧度 |
| `calc_tan(x)` | 正切函数 | 参数为弧度 |

### ui.h / ui.c — 交互界面
| 函数 | 说明 |
|------|------|
| `ui_show_menu()` | 显示主菜单 |
| `ui_get_number(*num)` | 获取单个数字（含输入校验） |
| `ui_get_input(*a, *b)` | 获取两个操作数 |
| `ui_get_expression(buf, size)` | 获取表达式字符串 |
| `ui_print_result(result)` | 打印结果（自动裁剪尾零，避免科学计数法） |
| `ui_print_error(msg)` | 打印错误信息 |
| `ui_clear()` | 清屏并提示已清零 |

### parser.h / parser.c — 表达式解析器
- 递归下降解析器，支持完整运算符优先级
- **文法定义**：
  ```
  expression  → term (('+' | '-') term)*
  term        → factor (('*' | '/') factor)*
  factor      → NUMBER | '(' expression ')' | func_call | ('+' | '-') factor
  func_call   → 'sqrt' '(' expression ')' | 'pow' '(' expression ',' expression ')'
              | 'sin' '(' expression ')' | 'cos' '(' expression ')' | 'tan' '(' expression ')'
  ```
- **支持的内置函数**：`sqrt(x)` 开方、`pow(a, b)` 幂运算、`sin(x)` 正弦、`cos(x)` 余弦、`tan(x)` 正切
- **运算符优先级**：`()` > `sqrt/pow` > `* /` > `+ -`

## 已实现功能清单

| 编号 | 功能 | 菜单选项 | 说明 |
|------|------|----------|------|
| 1 | 加法 | [1] | 两个数相加 |
| 2 | 减法 | [2] | 两个数相减 |
| 3 | 乘法 | [3] | 两个数相乘 |
| 4 | 除法 | [4] | 两个数相除，除零报错 |
| 5 | 清零 | [5] | 清屏 |
| 6 | 表达式计算 | [6] | 支持括号、运算符优先级、函数嵌套 |
| 7 | 开方 | [7] | 单个数开方，负数报错 |
| 8 | 幂运算 | [8] | 底数和指数两个数 |
| 9 | 退出 | [0] | 退出程序 |
| 10 | sin | [9] | 正弦函数，参数为弧度 |
| 11 | cos | [10] | 余弦函数，参数为弧度 |
| 12 | tan | [11] | 正切函数，参数为弧度 |

### 表达式功能示例

```
(1+2)*3         → 9
10/2+3*4        → 17
((1+2)*(4-1))   → 9
sqrt(16)        → 4
pow(2, 10)      → 1024
pow(sqrt(9), 2) → 9
sin(0)             → 0
cos(0)             → 1
tan(3.14159/4)     → 1
sqrt(sin(3.14159/2) + 1) → 1.41421
```
