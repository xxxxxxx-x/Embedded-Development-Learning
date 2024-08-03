/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0718/homework7.c
 * @brief        :
 *                  作业：一个有N个元素的整型数组，求该数组的各个子数组中，子数组之和的最大值是多少？
 *                  例如数组a[7] = {-2, 5, 3, -6, 4, -8, 6};
 *                  则子数组之和的最大值是 8 (即 a[1] + a[2])。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-19 09:33:18
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 函数声明，使用Kadane算法求最大子数组和
int maxSubArraySum(int arr[], int size);

int main(int argc, const char *argv[]) {
  // 示例数组
  int a[7] = {-2, 5, 3, -6, 4, -8, 6};
  int n = sizeof(a) / sizeof(a[0]);

  // 计算最大子数组和
  int max_sum = maxSubArraySum(a, n);

  // 输出结果
  printf("子数组之和的最大值是 %d\n", max_sum);

  return 0;
}

// 使用Kadane算法计算最大子数组和
int maxSubArraySum(int arr[], int size) {
  int max_so_far = arr[0];
  int max_ending_here = arr[0];

  for (int i = 1; i < size; i++) {
    if (max_ending_here < 0) {
      max_ending_here = arr[i];
    } else {
      max_ending_here += arr[i];
    }

    if (max_so_far < max_ending_here) {
      max_so_far = max_ending_here;
    }
  }

  return max_so_far;
}
