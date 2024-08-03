/****************************************************************************************
 * @file         : demo21.c
 * @brief        :
 *小练习：有一分数序列：2/1，3/2，5/3，8/5，13/8，21/13...求出这个数列的前20项之和。
 *                 这个序列是通过每项的分子和分母分别作为斐波那契数列生成的。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-16 10:50:36
 * @version      : 1.0
 * @note         :
 *                 本代码计算并输出给定分数序列的前20项之和。
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  double sum = 0.0;
  double numerator = 2.0;   // 分子
  double denominator = 1.0; // 分母
  double temp;              // 用于交换的临时变量

  // 计算前20项的和
  for (int i = 1; i <= 20; i++) {
    sum += numerator / denominator;

    // 更新分子和分母，生成下一个斐波那契数
    temp = numerator;
    numerator = numerator + denominator;
    denominator = temp;
  }

  printf("前20项之和是：%lf\n", sum);
  return 0;
}
