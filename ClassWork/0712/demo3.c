/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0712/demo3.c
 * @brief        : 小练习：利用while语句求出所有的“水仙花数”
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-12 15:48:13
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  int number = 100;          // 初始化从100开始的三位数
  int units, tens, hundreds; // 定义个位、十位、百位数字

  // 打印所有水仙花数
  printf("所有水仙花数分别 ==> ");
  while (number <= 999) {
    units = number % 10;       // 计算个位数
    tens = (number / 10) % 10; // 计算十位数
    hundreds = number / 100;   // 计算百位数

    // 判断是否为水仙花数
    if (units * units * units + tens * tens * tens +
            hundreds * hundreds * hundreds ==
        number) {
      printf("%d    ", number); // 输出水仙花数
    }

    number++; // 递增数字
  }
  printf("\n");

  return 0;
}
