/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0802/demo2/kernel_list.c
 * @brief        : 测试内核链表功能
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-02 15:33:23
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "kernel_list.h"
#include <stdio.h>

int main(int argc, const char *argv[]) {
  // 定义并初始化链表头
  DEFINE_LIST_HEAD(head);

  // 定义并初始化链表节点
  DEFINE_LIST_HEAD(node0);
  DEFINE_LIST_HEAD(node1);
  DEFINE_LIST_HEAD(node2);
  DEFINE_LIST_HEAD(node3);
  DEFINE_LIST_HEAD(node4);
  DEFINE_LIST_HEAD(node5);
  DEFINE_LIST_HEAD(node6);
  DEFINE_LIST_HEAD(node7);
  DEFINE_LIST_HEAD(node8);
  DEFINE_LIST_HEAD(node9);
  DEFINE_LIST_HEAD(node10);

  // 将节点添加到链表中
  list_add(&node0, &head);
  list_add(&node1, &head);
  list_add(&node2, &head);
  list_add(&node3, &head);
  list_add(&node4, &head);
  list_add(&node5, &head);
  list_add(&node6, &head);
  list_add(&node7, &head);
  list_add(&node8, &head);
  list_add(&node9, &head);
  list_add(&node10, &head);

  printf("添加\n");
  struct list_head *pos;
  list_for_each(pos, &head) { printf("%p\n", pos); }

  list_del(&node0);
  list_del(&node1);
  list_del(&node2);
  list_del(&node3);
  list_del(&node4);
  list_del(&node5);
  list_del(&node6);
  list_del(&node7);
  list_del(&node8);
  list_del(&node9);
  list_del(&node10);

  printf("删除\n");
  list_for_each(pos, &head) { printf("%p\n", pos); }

  //……

  return 0;
}
