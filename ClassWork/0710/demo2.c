/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0710/demo2.c
 * @brief        : 小练习:获取用户输入的一个数值，并输出它对应的字符
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-10 14:31:46
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2023-2024   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

int main(int argc, const char *argv[]) {

  unsigned short int value;            //声明一个字符型空间value
  printf("请输入字符:"); //提示用户输入字符
  scanf("%hu", &value);   // 读取用户输入的数字
  printf("%d对应的ASCII码是：%c\n", value, value); //修正输出格式
  return 0;
}
