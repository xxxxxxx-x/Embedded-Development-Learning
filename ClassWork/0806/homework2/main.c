/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0806/homework2/main.c
 * @brief        : 作业：查询希尔排序的实现原理并尝试实现
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-06 20:10:11
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 打印数组
void printArray(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

// 希尔排序函数
void shellSort(int arr[], int n) {
  // 开始以数组长度的一半作为初始间隔
  for (int gap = n / 2; gap > 0; gap /= 2) {
    printf("当前间隔: %d\n", gap); // 输出当前的间隔

    // 对每个间隔进行插入排序
    for (int i = gap; i < n; i++) {
      int temp = arr[i];
      int j;
      // 移动间隔元素使其有序
      for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
        arr[j] = arr[j - gap];
        printf("移动元素: %d -> %d\n", arr[j], arr[j - gap]); // 输出移动的元素
      }
      arr[j] = temp;
      printf("插入元素: %d 在位置 %d\n", temp, j); // 输出插入的元素及其位置
    }
    // 每次间隔排序后的数组状态
    printf("间隔 %d 排序后的数组: ", gap);
    printArray(arr, n);
  }
}

// 主函数
int main() {
  int arr[] = {12, 34, 54, 2, 3};
  int n = sizeof(arr) / sizeof(arr[0]);

  printf("排序前的数组: \n");
  printArray(arr, n);

  shellSort(arr, n);

  printf("排序后的数组: \n");
  printArray(arr, n);

  return 0;
}
