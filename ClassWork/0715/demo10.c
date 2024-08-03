/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0715/demo10.c
 * @brief        : 小练习：打印菱形图形
 *                  *
 *                 ***
 *                *****
 *               *******
 *                *****
 *                 ***
 *                  *
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-15 17:23:53
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main() {
  unsigned int num_rows;

  printf("请输入一个正整数：");
  scanf("%u", &num_rows);

  // 打印菱形的上半部分
  for (int i = 1; i <= num_rows; i++) {
    // 打印空格
    for (int j = 1; j <= num_rows - i; j++) {
      printf(" ");
    }
    // 打印星号
    for (int j = 1; j <= 2 * i - 1; j++) {
      printf("*");
    }
    printf("\n");
  }

  // 打印菱形的下半部分
  for (int i = num_rows - 1; i >= 1; i--) {
    // 打印空格
    for (int j = 1; j <= num_rows - i; j++) {
      printf(" ");
    }
    // 打印星号
    for (int j = 1; j <= 2 * i - 1; j++) {
      printf("*");
    }
    printf("\n");
  }

  return 0;
}
