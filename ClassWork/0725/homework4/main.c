/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0725/homework4/main.c
 * @brief        :
 *                 作业：声明4个函数，并把一个指针数组初始化为指向它们。每个函数接受两个double参数并返回一个double
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-26 08:20:58
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 声明四个函数，每个函数接受两个 double 参数并返回一个 double 值
double add(double a, double b) { return a + b; }

double subtract(double a, double b) { return a - b; }

double multiply(double a, double b) { return a * b; }

double divide(double a, double b) {
  if (b != 0) {
    return a / b;
  } else {
    printf("错误：除数不能为零\n");
    return 0; // 返回一个默认值，实际应用中可能需要更合适的处理方式
  }
}

int main() {
  // 声明一个指针数组，初始化为指向四个函数的指针
  double (*func_ptrs[4])(double, double) = {add, subtract, multiply, divide};

  // 示例使用：调用每个函数并输出结果
  double x = 10.0, y = 5.0;
  for (int i = 0; i < 4; ++i) {
    printf("结果 %d: %.2f\n", i + 1, func_ptrs[i](x, y));
  }

  return 0;
}
