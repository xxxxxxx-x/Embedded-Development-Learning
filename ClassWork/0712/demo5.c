/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0712/demo5.c
 * @brief        :
 *                 小练习：利用switch语句，获取用户输入的1~7，并根据1~7输出“星期一”到“星期天”
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-12 17:05:15
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  unsigned char dayOfWeek;

  // 提示用户输入1~7的数字
  printf("请输入一个1到7之间的数字: ");
  scanf("%hhu", &dayOfWeek);

  // 根据用户输入的数字输出对应的星期
  switch (dayOfWeek) {
  case 1:
    printf("星期一\n");
    break;
  case 2:
    printf("星期二\n");
    break;
  case 3:
    printf("星期三\n");
    break;
  case 4:
    printf("星期四\n");
    break;
  case 5:
    printf("星期五\n");
    break;
  case 6:
    printf("星期六\n");
    break;
  case 7:
    printf("星期天\n");
    break;
  default:
    // 输入超出范围，提示输入错误
    printf("输入错误，请输入1到7之间的数字\n");
    break;
  }

  return 0;
}
