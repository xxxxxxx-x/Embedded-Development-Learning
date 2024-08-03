/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0715/demo8.c
 * @brief        :
 *                 小练习：编写一个程序，接受一个整数输入，然后显示所有小于或等于该数的素数。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-15 16:17:51
 * @version      : 1.0
 * @note         :
 *                 - 使用数组存储已经确认的素数，以减少重复检查。
 *                 - 优化输出格式，每行输出10个素数。
 *****************************************************************************************************************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
  unsigned int number;
  printf("请输入一个整数：");
  scanf("%u", &number);

  // 如果输入的数小于2，则直接输出无素数
  if (number < 2) {
    printf("没有小于或等于 %u 的素数。\n", number);
    return 0;
  }

  int primes[number]; // 存储素数的数组
  int count = 0;      // 素数计数器

  for (int i = 2; i <= number; i++) {
    bool isPrime = true;

    // 检查 i 是否为素数
    for (int j = 0; j < count; j++) {
      if (i % primes[j] == 0) {
        isPrime = false;
        break;
      } else if (i < sqrt(primes[j])) {
        isPrime = false;
        break;
      }
    }

    if (isPrime) {
      primes[count++] = i; // 将素数添加到数组中
      printf("%-5d ", i);

      // 每行输出10个素数，换行
      if (count % 10 == 0) {
        printf("\n");
      }
    }
  }

  printf("\n");
  return 0;
}
