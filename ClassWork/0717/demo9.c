/***********************************************************************************************
 * @file         : demo9.c
 * @brief        : 小练习：将一个 int 拆分成两个 short
 * @date         : 2024-07-17
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main() {
  int value = 12345678;

  // 使用位运算拆分整数为两个 short
  short int value1 = (value & 0xFFFF);         // 获取低位的 short
  short int value2 = ((value >> 16) & 0xFFFF); // 获取高位的 short

  printf("原整数值为：%0#10x\n", value);           // 输出原整数值
  printf("拆分后的 short 值为：%0#10x\n", value1); // 输出第一个 short 值
  printf("拆分后的 short 值为：%0#10x\n", value2); // 输出第二个 short 值

  return 0;
}
