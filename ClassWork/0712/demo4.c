/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0712/demo4.c
 * @brief        :
 *                 小练习：利用循环，把一个无符号的十进制数据转换为无符号十六进制数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-12 16:26:58
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  unsigned int decimal;
  char hex[9]; // 存储转换后的十六进制字符串
  int index = 0;

  // 提示用户输入一个无符号的十进制数
  printf("请任意输入一个无符号的十进制: "); // 例如输入16
  scanf("%u", &decimal);

  // 特殊情况：输入为0时，直接输出0
  if (decimal == 0) {
    printf("0x00\n");
    return 0;
  }

  // 初始化十六进制前缀
  printf("0x");

  // 循环将十进制数转换为十六进制数
  while (decimal > 0) {
    int remainder = decimal % 16; // 计算当前位的余数
    hex[index++] = (remainder > 9) ? (65 + remainder - 10) : (48 + remainder); // 将余数转换为对应的十六进制字符
    decimal = decimal / 16; // 更新十进制数
  }

  // 反向输出十六进制数
  for (int i = index - 1; i >= 0; i--) {
    printf("%c", hex[i]);
  }
  printf("\n");

  return 0;
}
