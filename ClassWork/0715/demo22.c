/****************************************************************************************
 * @file         : demo22.c
 * @brief        : 输入某年某月某日，判断这一天是这一年的第几天。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-16 11:07:45
 * @version      : 1.0
 * @note         :
 *                 本代码根据用户输入的日期，计算并输出该日期是这一年的第几天。
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************/

#include <stdbool.h>
#include <stdio.h>

// 判断是否是闰年
bool isLeapYear(unsigned int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int main(int argc, const char *argv[]) {
  unsigned int year, month, day;
  unsigned int dayOfYear = 0;

  // 定义每个月的天数
  unsigned int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  // 获取用户输入的日期
  printf("请输入一个某年某月某日 (格式: XXXX年X月X日)：");
  scanf("%u年%u月%u日", &year, &month, &day);

  // 检查闰年并调整二月的天数
  if (isLeapYear(year)) {
    daysInMonth[1] = 29;
  }

  // 计算该日期是当年的第几天
  for (unsigned int i = 0; i < month - 1; i++) {
    dayOfYear += daysInMonth[i];
  }
  dayOfYear += day;

  // 输出结果
  printf("%u年%u月%u日是%s，是一年的第%u天\n", year, month, day,
         isLeapYear(year) ? "闰年" : "平年", dayOfYear);

  return 0;
}
