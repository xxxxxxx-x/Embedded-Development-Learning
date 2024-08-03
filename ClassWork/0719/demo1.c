/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0719/demo1.c
 * @brief        : 小练习：指针和指针的指针示例
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-19 11:15:59
 * @version      : 1.0
 * @note         : 通过示例演示指针和指针的指针的用法
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Rights Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

int main() {

  int value = 10;   // 定义一个整数变量 value 并初始化为 10
  int *p = &value;  // 定义一个指针 p，指向 value 的地址
  int **q = &p;     // 定义一个指针 q，指向指针 p 的地址
  int data = 10086; // 定义一个整数变量 data 并初始化为 10086

  // 打印指针 p 指向的值
  printf("%d\n", *p); // 输出 10

  // 打印指针 q 指向的指针 p 指向的值，即 value 的值
  printf("%d\n", **q); // 输出 10

  // 打印指针 p 的地址，即 value 的地址
  printf("%p\n", (void *)p); // 输出 &value

  // 打印指针 q 指向的指针 p 的地址，即 value 的地址
  printf("%p\n", (void *)*q); // 输出 &value

  // 修改指针 p，使其指向 data 的地址
  *q = &data; // 现在 p 指向 data 的地址

  // 修改指针 p 指向的值，即 data 的值
  *p = 10010; // data 现在的值变为 10010

  // 打印 value 和 data 的值
  printf("value=%d\n", value); // 输出 10（value 没有被修改）
  printf("data=%d\n", data);   // 输出 10010（data 被修改）

  // 修改指针 q 指向的指针 p 指向的值，即 data 的值
  **q = 123456; // data 的值变为 123456

  // 再次打印 value 和 data 的值
  printf("value=%d\n", value); // 输出 10（value 依旧没有被修改）
  printf("data=%d\n", data);   // 输出 123456（data 被修改）

  return 0;
}
