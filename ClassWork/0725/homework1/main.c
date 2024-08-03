/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0725/homework1/main.c
 * @brief        : 作业：定义并打印月份信息的示例程序
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-25 17:13:50
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 定义结构体来保存月份信息
struct MonthInfo {
  char name[15];        // 月份名
  char abbreviation[4]; // 3个字母的缩写
  int days;             // 该月的天数
  int month_number;     // 月份号
};

int main() {

  struct MonthInfo months[] = {
      {"一月", "Jan", 31, 1},    {"二月", "Feb", 28, 2}, // 闰年时为29天
      {"三月", "Mar", 31, 3},    {"四月", "Apr", 30, 4},
      {"五月", "May", 31, 5},    {"六月", "Jun", 30, 6},
      {"七月", "Jul", 31, 7},    {"八月", "Aug", 31, 8},
      {"九月", "Sep", 30, 9},    {"十月", "Oct", 31, 10},
      {"十一月", "Nov", 30, 11}, {"十二月", "Dec", 31, 12}};

  // 示例用法：访问特定月份的信息
  printf("月份: %s\n", months[0].name);
  printf("缩写: %s\n", months[0].abbreviation);
  printf("天数: %d\n", months[0].days);
  printf("月份号: %d\n", months[0].month_number);

  return 0;
}
