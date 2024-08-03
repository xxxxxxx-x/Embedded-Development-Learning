/*******************************************************************************************************
 * @file         : demo24.c
 * @brief        : 输入两个正整数m和n，求其最大公约数和最小公倍数。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-16 15:46:51
 * @version      : 1.0
 * @note         :
 *本代码使用欧几里得算法求最大公约数，并通过最大公约数计算最小公倍数。
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *******************************************************************************************************/

#include <stdio.h>

// 计算最大公约数的函数，使用欧几里得算法
unsigned int gcd(unsigned int a, unsigned int b) {
  while (b != 0) {
    unsigned int temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

// 计算最小公倍数的函数，基于最大公约数
unsigned int lcm(unsigned int a, unsigned int b, unsigned int gcd) {
  return (a * b) / gcd;
}

int main(int argc, const char *argv[]) {
  unsigned int m, n;
  printf("请输入两个正整数m和n(空格分割): ");
  scanf("%u %u", &m, &n);

  unsigned int gcd_value = gcd(m, n);
  unsigned int lcm_value = lcm(m, n, gcd_value);

  printf("最大公约数是：%u, 最小公倍数是：%u\n", gcd_value, lcm_value);
  return 0;
}
