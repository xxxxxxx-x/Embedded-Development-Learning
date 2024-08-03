/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0801/homework1/main.c
 * @brief        : 测试双链表的功能
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-01 16:30:00
 * @version      : 1.0
 * @note         : 包含创建、插入、删除、更新和查询节点的测试示例
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "dlist.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  DListNode *head = NULL; // 双链表头指针初始化为空

  // 测试插入操作
  printf("插入操作测试：\n");

  // 在链表头部插入节点
  insert_dnode_head(&head, 10);
  insert_dnode_head(&head, 20);
  insert_dnode_head(&head, 30);
  printf("链表头部插入节点后：");
  display_dlist(head); // 期望输出：30 20 10

  // 在链表尾部插入节点
  insert_dnode_tail(&head, 40);
  insert_dnode_tail(&head, 50);
  printf("链表尾部插入节点后：");
  display_dlist(head); // 期望输出：30 20 10 40 50

  // 在指定位置插入节点
  insert_dnode_at(&head, 25, 2);
  printf("在位置 2 插入节点 25 后：");
  display_dlist(head); // 期望输出：30 20 25 10 40 50

  // 测试更新操作
  update_dnode_value(head, 2, 100);
  printf("更新位置 2 节点的值为 100 后：");
  display_dlist(head); // 期望输出：30 20 100 10 40 50

  // 测试删除操作
  delete_dnode_head(&head);
  printf("删除头节点后：");
  display_dlist(head); // 期望输出：20 100 10 40 50

  delete_dnode_tail(&head);
  printf("删除尾节点后：");
  display_dlist(head); // 期望输出：20 100 10 40

  delete_dnode_at(&head, 1);
  printf("删除位置 1 的节点后：");
  display_dlist(head); // 期望输出：20 10 40

  // 测试查找操作
  DListNode *node = search_dnode_value(head, 10);
  if (node != NULL) {
    printf("找到值 10 的节点：%d\n", node->data); // 期望输出：10
  } else {
    printf("未找到值 10 的节点\n");
  }

  node = search_dnode_value(head, 100);
  if (node != NULL) {
    printf("找到值 100 的节点：%d\n", node->data);
  } else {
    printf("未找到值 100 的节点\n"); // 期望输出：未找到值 100 的节点
  }

  // 释放链表所有节点的内存
  while (head != NULL) {
    delete_dnode_head(&head);
  }

  return 0;
}
