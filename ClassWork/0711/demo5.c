/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0711/demo5.c
 * @brief        : 计算下述表达式的结果，并以二进制形式输出
 *                 int value1 = 0xab;
 *                 int value2 = 0xcd;
 *                 int tmp1 = value1 & value2; // 按位与运算
 *                 int tmp2 = value1 | value2; // 按位或运算
 *                 int tmp3 = value1 ^ value2; // 按位异或运算
 *                 输出以上三个变量的二进制表示形式
 *                 例如：
 *                 value1=10101011,value2=11001101
 *                 tmp1=10001011,tmp2=11101101,tmp3=01100110
 *
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-11 14:41:23
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 函数声明，将整数转换为二进制字符串
void int_to_binary_string(int num, char *binary_string, int size);

int main() {
  // 定义变量并初始化
  int value1 = 0xab;          // 0xab 对应二进制 10101011
  int value2 = 0xcd;          // 0xcd 对应二进制 11001101
  int tmp1 = value1 & value2; // 按位与运算结果
  int tmp2 = value1 | value2; // 按位或运算结果
  int tmp3 = value1 ^ value2; // 按位异或运算结果

  // 定义存放二进制字符串的数组
  char s[9]; // 8位二进制数 + '\0'

  // 输出 value1 的二进制形式
  int_to_binary_string(value1, s, 8);
  printf("value1\t==>\t%s\n", s);

  // 输出 value2 的二进制形式
  int_to_binary_string(value2, s, 8);
  printf("value2\t==>\t%s\n", s);

  // 输出 tmp1 的二进制形式
  int_to_binary_string(tmp1, s, 8);
  printf("单与\t==>\t%s\n", s);

  // 输出 tmp2 的二进制形式
  int_to_binary_string(tmp2, s, 8);
  printf("单或\t==>\t%s\n", s);

  // 输出 tmp3 的二进制形式
  int_to_binary_string(tmp3, s, 8);
  printf("单亦或\t==>\t%s\n", s);

  return 0;
}

// 将整数转换为指定位数的二进制字符串
void int_to_binary_string(int num, char *binary_string, int size) {
  int i;
  for (i = size - 1; i >= 0; --i) {
    binary_string[i] = (num & 1) ? '1' : '0';
    num >>= 1;
  }
  binary_string[size] = '\0'; // 添加字符串结束符
}