/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0723/demo1/calculator.c
 * @brief        : 计算器函数实现
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-23 10:18:57
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>

// 实现加法
double Addition(double num1, double num2) { return num1 + num2; }

// 实现减法
double Subtraction(double num1, double num2) { return num1 - num2; }

// 实现乘法
double Multiplication(double num1, double num2) { return num1 * num2; }

// 实现除法
double Division(double num1, double num2) {
  if (num2 == 0) {
    // 如果除数为0，打印错误信息并退出程序
    printf("发生算术异常！\n");
    exit(1);
  }
  return num1 / num2;
}
