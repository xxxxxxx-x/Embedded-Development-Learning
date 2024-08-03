/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0724/demo4/main.c
 * @brief        : 小练习：计算结构体大小
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-24 14:50:27
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

// #pragma pack(1)
#include <stdio.h> // 包含标准输入输出头文件

// 定义第一个结构体
struct Test1 {
  char dog;
  char fox;
  unsigned short pig;
  unsigned long cat;
};

// 定义第二个结构体
struct Test2 {
  char dog;
  unsigned long cat;
  unsigned short pig;
  char fox;
};

int main() {
  // 打印结构体 Test1 的大小
  printf("Size of struct Test1: %lu bytes\n", sizeof(struct Test1));

  // 打印结构体 Test2 的大小
  printf("Size of struct Test2: %lu bytes\n", sizeof(struct Test2));

  return 0;
}
