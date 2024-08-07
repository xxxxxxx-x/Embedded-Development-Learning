/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0806/demo3/main.c
 * @brief        : 对数组进行降序排序并打印结果
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-06 15:59:00
 * @version      : 1.0
 * @note         : 本程序对一个整数数组进行降序排序，并输出排序后的结果。
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  // 定义并初始化一个整数数组
  int array[] = {1, 123, 1245, 63, 63, 71, 63, 2};
  int arraySize = sizeof(array) / sizeof(array[0]); // 计算数组的元素个数

  // 使用选择排序算法对数组进行降序排序
  for (int i = 0; i < arraySize; i++) {
    for (int j = i + 1; j < arraySize; j++) {
      if (array[j] > array[i]) {
        // 交换 array[i] 和 array[j]
        int temp = array[j];
        array[j] = array[i];
        array[i] = temp;
      }
    }
  }

  // 打印排序后的数组
  printf("排序后的数组（降序）：");
  for (int i = 0; i < arraySize; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");

  return 0;
}
