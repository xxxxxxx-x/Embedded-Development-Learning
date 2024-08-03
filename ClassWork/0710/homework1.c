/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0710/homework1.c
 * @brief        :
 *                 作业1：一年大约有 3.1536*107s。编写一个程序，要求输入你的年龄，然后显示该年龄等于多少秒。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-10 16:46:39
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2023-2024   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdbool.h>
#include <stdio.h>
int main(int argc, const char *argv[]) {
  signed short int year;
  while (true) {
    printf("请输入你的年龄：");
    scanf("%hd", &year);
    if (year > 0) {
      break;
    }
    while (getchar() != '\n')
      ;
    printf("年龄不合法，请重新输入！\n");
  }

  printf("你已经活了%u秒\n", 60 * 60 * 24 * 365 * year);

  return 0;
}