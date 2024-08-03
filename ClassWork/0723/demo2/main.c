/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0723/demo2/main.c
 * @brief        : 小练习：编写一个内联函数，交换两个数据的值。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-23 14:20:05
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 内联函数声明
inline void swap(int *num1, int *num2);

// 内联函数实现
inline void swap(int *num1, int *num2) {
  int tmp = *num1;
  *num1 = *num2;
  *num2 = tmp;
}

int main(int argc, const char *argv[]) {
  int num1 = 10, num2 = 20;
  // 交换前输出
  printf("交换前：num1=%d, num2=%d\n", num1, num2);
  // 调用交换函数
  swap(&num1, &num2);
  // 交换后输出
  printf("交换后：num1=%d, num2=%d\n", num1, num2);
  return 0;
}
