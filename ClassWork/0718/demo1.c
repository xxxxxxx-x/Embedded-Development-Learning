/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0718/demo1.c
 * @brief        :
 *                 小练习：定义一个指针，正确指向上述的数组，并利用指针遍历所有元素
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-18 14:45:03
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  // 定义一个二维数组并初始化
  int array[5][3] = {
      {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}};

  // 定义一个指向数组首元素的指针
  int *ptr = &array[0][0];

  // 遍历二维数组的所有元素
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      // 使用指针访问数组元素
      printf("array[%d][%d] => %d ", i, j, *(ptr + (i * 3 + j)));
    }
    printf("\n"); // 每行输出结束后换行
  }

  return 0;
}
