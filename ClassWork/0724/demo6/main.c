/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0724/demo6/main.c
 * @brief        : 小练习：编写一个宏调用 add_int 和 add_float 函数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-24 16:53:08
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h> // 包含标准输入输出头文件

// 定义宏来调用 add_int 和 add_float 函数
#define ADD(type) add_##type
#define PRINT(value, format) printf(format, value) 

// 定义整型相加函数
int add_int(int a, int b) { return a + b; }

// 定义浮点型相加函数
float add_float(float a, float b) { return a + b; }

int main(int argc, const char *argv[]) {
  // 使用宏调用整型相加函数
  int i = ADD(int)(10, 20);
  PRINT(i, "%d\n"); // 修正输出格式为整型

  // 使用宏调用浮点型相加函数
  float f = ADD(float)(10.5, 20.3);
  PRINT(f, "%f\n"); // 输出格式为浮点型

  return 0;
}
