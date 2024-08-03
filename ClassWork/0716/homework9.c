/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/homework9.c
 * @brief        : 作业：计算一个二维数组中边线的和。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-16 21:54:12
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

int main() {
  // 定义二维数组和边线和变量
  int array[5][5] = {{1, 2, 3, 4, 5},
                     {1, 2, 3, 4, 5},
                     {1, 2, 3, 4, 5},
                     {1, 2, 3, 4, 5},
                     {1, 2, 3, 4, 5}};
  int sum = 0;

  // 初始化边界索引
  int y_start = 0, x_start = 0;
  int y_end = sizeof(array) / sizeof(array[0]) - 1;
  int x_end = sizeof(array[0]) / sizeof(int) - 1;

  // 计算上边线和
  for (int i = x_start; i <= x_end; i++) {
    sum += array[y_start][i];
  }
  y_start++;

  // 计算右边线和
  for (int i = y_start; i <= y_end; i++) {
    sum += array[i][x_end];
  }
  x_end--;

  // 计算下边线和
  for (int i = x_end; i >= x_start; i--) {
    sum += array[y_end][i];
  }
  y_end--;

  // 计算左边线和
  for (int i = y_end; i >= y_start; i--) {
    sum += array[i][x_start];
  }
  x_start++;

  // 输出边线和
  printf("该二维数组边线的和是：%d\n", sum);

  return 0;
}
