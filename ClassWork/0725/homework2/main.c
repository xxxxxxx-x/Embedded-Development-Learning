/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0725/homework2/main.c
 * @brief        :
 *                 作业：定义一个结构体变量（包含年月日）。计算该日在本年中是第几天？注意闰年问题。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-26 08:04:22
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

// 定义结构体表示日期
struct Date {
  int year;
  int month;
  int day;
};

// 判断是否闰年的函数
int isLeapYear(int year) {
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    return 1; // 是闰年
  } else {
    return 0; // 不是闰年
  }
}

// 计算给定日期在本年中是第几天的函数
int dayOfYear(struct Date date) {
  // 每个月的天数，依次是非闰年和闰年
  int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int i, days = 0;

  // 如果是闰年，二月份天数为29天
  if (isLeapYear(date.year)) {
    daysInMonth[1] = 29;
  }

  // 累加前面几个月的天数
  for (i = 0; i < date.month - 1; i++) {
    days += daysInMonth[i];
  }

  // 加上本月的天数
  days += date.day;

  return days;
}

int main() {
  struct Date date;

  // 输入日期
  printf("请输入日期（年 月 日，用空格分隔）: ");
  scanf("%d %d %d", &date.year, &date.month, &date.day);

  // 计算天数
  int day = dayOfYear(date);

  // 输出结果
  printf("%d-%d-%d 是本年的第 %d 天。\n", date.year, date.month, date.day, day);

  return 0;
}
