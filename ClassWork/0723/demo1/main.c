/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0723/demo1/main.c
 * @brief        :
 *                 小练习：编写一个计算器源文件和头文件，实现（加、减、乘、除），并在main函数中调用
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-23 10:16:18
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "calculator.h"
#include <stdio.h>

int main(int argc, const char *argv[]) {
  // 调用各个计算函数并打印结果
  printf("加法结果：%lf\n", Addition(10, 20));
  printf("减法结果：%lf\n", Subtraction(10, 20));
  printf("乘法结果：%lf\n", Multiplication(10, 20));
  printf("除法结果：%lf\n", Division(10, 20));
  // 测试除以零的情况
  printf("除法结果：%lf\n", Division(10, 0));
  return 0;
}
