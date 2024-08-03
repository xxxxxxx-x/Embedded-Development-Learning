/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0711/demo3.c
 * @brief        : 判断一个数据是否为水仙花数。
 *                 水仙花数是一个三位数的数字，每个位的立方和等于该数本身。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-11 14:04:52
 * @version      : 1.0
 * @note         : 水仙花数示例：153 = 1^3 + 5^3 + 3^3
 * @Copyright    : (c)   2023-2024   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  // 声明变量
  signed short int number, units, tens, hundreds;
  
  // 提示用户输入一个三位数
  printf("请输入一个任意三位数，判断是否是水仙花数：");
  
  // 接收用户输入
  scanf("%hd", &number);
  
  // 分解数字的个位、十位、百位
  units = number % 10;        // 个位
  tens = (number / 10) % 10;  // 十位
  hundreds = number / 100;    // 百位
  
  // 判断是否为水仙花数
  if (units * units * units + tens * tens * tens + hundreds * hundreds * hundreds == number) {
    printf("是水仙花数\n");
  } else {
    printf("不是水仙花数\n");
  }

  return 0;
}
