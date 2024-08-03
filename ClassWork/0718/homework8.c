/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0718/homework8.c
 * @brief        :
 *                 作业：编写一个程序，初始化一个 3x5 的二维double型数组，
 *                 并利用一个基于变长数组的函数把该数组赋值到另一个二维数组，
 *                 另外再写一个基于变长数组的函数来显示两个数组的内容。
 *                 这两个函数应该能够处理任意的NxM 数组。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-19 09:40:10
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 函数声明
void copyArray(int rows, int cols, double source[rows][cols],
               double target[rows][cols]);
void displayArray(int rows, int cols, double arr[rows][cols]);

int main(int argc, const char *argv[]) {
  // 初始化3x5的二维double型数组
  double arr[3][5] = {{1.0, 2.0, 3.0, 4.0, 5.0},
                      {6.0, 7.0, 8.0, 9.0, 10.0},
                      {11.0, 12.0, 13.0, 14.0, 15.0}};

  // 声明一个3x5的目标二维数组
  double target[3][5];

  // 复制数组内容
  copyArray(3, 5, arr, target);

  // 显示源数组和目标数组内容
  printf("源数组:\n");
  displayArray(3, 5, arr);

  printf("目标数组:\n");
  displayArray(3, 5, target);

  return 0;
}

// 复制二维数组的内容
void copyArray(int rows, int cols, double source[rows][cols],
               double target[rows][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      target[i][j] = source[i][j];
    }
  }
}

// 显示二维数组的内容
void displayArray(int rows, int cols, double arr[rows][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%.1f ", arr[i][j]);
    }
    printf("\n");
  }
}
