/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework1/main.c
 * @brief        : 作业1： 计算一个链表的节点个数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-31 16:37:22
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************************************************************************
 * @name: count_nodes
 * @brief: 计算一个链表的节点个数
 * @param {ListNode*} head 链表头指针
 * @return {int} 链表节点的个数
 * @date: 2024-07-31 16:48:05
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
int count_nodes(ListNode *head) {
  // 如果链表为空，返回0
  if (head == NULL) {
    printf("链表为空，无法计算节点个数！\n");
    return 0;
  }

  // 计数器初始化为0
  int node_count = 0;
  // 临时指针指向链表头
  ListNode *current = head;

  // 遍历链表，直到链表末尾
  while (current != NULL) {
    node_count++;            // 计数器加1
    current = current->next; // 移动到下一个节点
  }

  // 返回节点个数
  return node_count;
}

int main() {
  ListNode *head = NULL;

  // 创建一个链表
  insert_head(&head, 10);
  insert_tail(&head, 20);
  insert_tail(&head, 30);
  insert_at(&head, 15, 1); // 在位置1插入15
  insert_at(&head, 25, 3); // 在位置3插入25

  // 显示链表
  printf("链表内容：");
  display_list(head);

  // 计算链表节点个数
  int length = count_nodes(head);
  printf("链表节点个数：%d\n", length);

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}