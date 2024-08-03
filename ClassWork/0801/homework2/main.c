/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0801/homework2/main.c
 * @brief        : 测试双循环链表的各项操作
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-01 17:22:47
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
 *****************************************************************************************************************************************************************/

#include "dclist.h"
#include <stdio.h>

int main() {
  DCListNode *head = NULL; // 初始化双循环链表头指针为空

  // 测试在链表头部插入节点
  printf("在链表头部插入节点: ");
  insert_dcnode_head(&head, 10);
  insert_dcnode_head(&head, 20);
  insert_dcnode_head(&head, 30);
  print_dclist_forward(head); // 期望输出: 30 20 10

  // 测试在链表尾部插入节点
  printf("\n在链表尾部插入节点: ");
  insert_dcnode_tail(&head, 40);
  insert_dcnode_tail(&head, 50);
  print_dclist_forward(head); // 期望输出: 30 20 10 40 50

  // 测试在指定位置插入节点
  printf("\n在位置 3 插入节点: ");
  insert_dcnode_at(&head, 25, 3);
  print_dclist_forward(head); // 期望输出: 30 20 25 10 40 50

  // 测试更新指定位置节点的值
  printf("\n将值 10 更新为 35: ");
  update_dcnode_value(head, 10, 35);
  print_dclist_forward(head); // 期望输出: 30 20 25 35 40 50

  // 测试删除头节点
  printf("\n删除头节点: ");
  delete_dcnode_head(&head);
  print_dclist_forward(head); // 期望输出: 20 25 35 40 50

  // 测试删除尾节点
  printf("\n删除尾节点: ");
  delete_dcnode_tail(&head);
  print_dclist_forward(head); // 期望输出: 20 25 35 40

  // 测试删除指定位置的节点
  printf("\n删除位置 1 的节点: ");
  delete_dcnode_at(&head, 1);
  print_dclist_forward(head); // 期望输出: 25 35 40

  // 测试搜索指定值的节点
  printf("\n搜索值为 35 的节点: ");
  DCListNode *node = search_dcnode_value(head, 35);
  if (node) {
    printf("找到节点，值为: %d\n", node->data); // 期望输出: 找到节点，值为:35
  } else {
    printf("未找到节点\n");
  }

  return 0;
}
