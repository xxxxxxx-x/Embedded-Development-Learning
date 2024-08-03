/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0730/demo2/main.c
 * @brief        :
 *                  小练习：把下面的三个堆空间链接起来，并用一个指针去遍历
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-30 15:23:25
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 定义链表节点结构体
struct node {
  int data;          // 存储数据
  struct node *next; // 指向下一个节点的指针
};

int main(int argc, const char *argv[]) {

  // 分配三个链表节点的堆空间
  struct node *p1 = (struct node *)malloc(sizeof(struct node));
  struct node *p2 = (struct node *)malloc(sizeof(struct node));
  struct node *p3 = (struct node *)malloc(sizeof(struct node));

  if (p1 == NULL || p2 == NULL || p3 == NULL) {
    // 检查内存分配是否成功
    printf("内存分配失败\n");
    return 1; // 返回错误码
  }

  // 初始化节点数据
  p1->data = 1;
  p2->data = 2;
  p3->data = 3;

  // 链接节点
  p1->next = p2;
  p2->next = p3;
  p3->next = NULL; // 最后一个节点的 next 指针设为 NULL

  // 定义指针用于遍历链表
  struct node *pos = p1;

  // 遍历并打印链表中的数据
  while (pos != NULL) {
    printf("%d\n", pos->data);
    pos = pos->next;
  }

  // 释放分配的内存
  free(p1);
  free(p2);
  free(p3);

  return 0;
}
