/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0725/homework5/main.c
 * @brief        :
 *                 编写一个transform()函数，它接受4个参数：包含double类型数据的源数组名，double类型的目标数组名，表示数组元素个数的int变量以及一个函数名（或者等价的指向函数的指针）。
 *                 transform()函数把指定的函数作为用于源数组的每一个元素，并返回值放到目标数组中。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-26 08:31:13
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

// 声明 transform 函数，参数分别为源数组、目标数组、元素个数和函数指针
void transform(double source[], double target[], int count,
               double (*func)(double)) {
  // 遍历源数组的每个元素
  for (int i = 0; i < count; ++i) {
    // 对源数组中的元素应用给定的函数，并将结果存入目标数组对应位置
    target[i] = func(source[i]);
  }
}

// 示例函数，将输入的数字平方并返回
double square(double x) { return x * x; }

// 主函数示例
int main() {
  double source[] = {1.0, 2.0, 3.0, 4.0};
  double target[4];
  int count = 4;

  // 调用 transform 函数，将 square 函数应用到 source 数组，结果存入 target 数组
  transform(source, target, count, square);

  // 输出转换后的目标数组
  printf("转换后的数组:\n");
  for (int i = 0; i < count; ++i) {
    printf("%.2f ", target[i]);
  }
  printf("\n");

  return 0;
}
