/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0727/exam1/main.c
 * @brief        :
 *                 试卷编程题：用C语言编写一个输入的整数，倒着输出整数的函数，要求用递归方法
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-29 10:28:40
 * @version      : 1.1
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 定义递归函数，用于倒着输出整数
void reversePrint(int num) {
  if (num == 0) {
    return;
  }
  printf("%d", num % 10); // 输出当前数字的最后一位
  reversePrint(num / 10); // 递归处理剩下的部分
}

int main(int argc, const char *argv[]) {
  signed int inputNumber;
  printf("请输入一个整数："); // 提示用户输入一个整数
  scanf("%d", &inputNumber);
  // 处理输入为0的情况
  if (inputNumber == 0) {
    printf("0");
  } else if (inputNumber < 0) {
    // 处理负数情况，先输出负号，然后处理绝对值部分
    printf("-");
    reversePrint(-inputNumber);
  } else {
    reversePrint(inputNumber);
  }
  printf("\n"); // 换行以美化输出
  return 0;
}
