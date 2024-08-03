/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0730/demo1/main.c
 * @brief        :
 *                  小练习：创建三个用于存储
 *                  float类型的内存空间，并把这三个空间利用指针链接起来，并利用第一个空间的地址访问另外两个空间的内容
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-30 14:28:40
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 定义链表结构体
struct Linked {
  float data; // 存储 float 类型数据
  void *next; // 指向下一个链表节点的指针
};

int main(int argc, const char *argv[]) {

  // 初始化三个 float 类型变量
  float f1 = 10.0;
  float f2 = 20.0;
  float f3 = 30.0;

  // 定义三个链表节点
  struct Linked link1;
  struct Linked link2;
  struct Linked link3;

  // 将变量值赋给链表节点
  link1.data = f1;
  link2.data = f2;
  link3.data = f3;

  // 链接链表节点
  link1.next = &link2;
  link2.next = &link3;
  link3.next = NULL; // 最后一个节点的 next 指针设为 NULL

  // 通过第一个链表节点访问另外两个节点的值
  printf("通过第一个链表访问另外两个节点的值 -> %.2f, %.2f\n",
         ((struct Linked *)link1.next)->data, // 访问第二个节点的值
         ((struct Linked *)((struct Linked *)link1.next)->next)
             ->data); // 访问第三个节点的值

  return 0;
}
