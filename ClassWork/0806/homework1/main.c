/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0806/homework1/main.c
 * @brief        : 作业：查询快速排序的实现原理并尝试实现
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-06 20:10:11
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 交换两个整数的值
void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

// 分区函数，返回基准元素的位置
int partition(int arr[], int low, int high) {
  int pivot = arr[high]; // 选择最后一个元素作为基准
  int i = (low - 1);     // i初始化为low - 1

  for (int j = low; j <= high - 1; j++) {
    // 如果当前元素小于或等于基准
    if (arr[j] <= pivot) {
      i++;                    // 增加i的值
      swap(&arr[i], &arr[j]); // 交换arr[i]和arr[j]
    }
  }
  swap(&arr[i + 1], &arr[high]); // 将基准元素放置在正确的位置
  return (i + 1);
}

// 快速排序函数
void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high); // 找到基准元素的位置

    // 分别对基准元素左边和右边的子数组进行递归排序
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

// 打印数组
void printArray(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

// 主函数
int main() {
  int arr[] = {10, 7, 8, 9, 1, 5};
  int n = sizeof(arr) / sizeof(arr[0]);

  printf("排序前的数组: \n");
  printArray(arr, n);

  quickSort(arr, 0, n - 1);

  printf("排序后的数组: \n");
  printArray(arr, n);
  return 0;
}
