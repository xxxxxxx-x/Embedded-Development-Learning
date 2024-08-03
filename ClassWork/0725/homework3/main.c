/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0725/homework3/main.c
 * @brief        :
 *                 作业：声明一个枚举类型，使用choices作为标记，将枚举常量no、yes和maybe分别设置为0、1和2
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-26 08:18:23
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 声明枚举类型 choices，包含常量 no、yes 和 maybe
enum choices { no = 0, yes = 1, maybe = 2 };

int main() {
  enum choices status = yes; // 声明并初始化一个枚举变量

  // 使用枚举变量，并输出对应的整数值
  printf("当前状态：%d\n", status);

  return 0;
}
