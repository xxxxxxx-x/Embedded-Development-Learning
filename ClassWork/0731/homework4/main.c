/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework4/main.c
 * @brief        : 作业4：找链表中的倒数第k个结点
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
 * @name: find_kth_from_end
 * @brief: 查找链表中的倒数第 k 个结点
 * @param {ListNode*} head 链表头指针
 * @param {int} k 倒数第 k 个位置
 * @return {ListNode*} 找到的节点指针，未找到返回 NULL
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
ListNode *find_kth_from_end(ListNode *head, int k) {
  // 检查链表是否为空
  if (head == NULL) {
    printf("提供的链表是NULL，查找失败！\n");
    return NULL;
  }

  ListNode *first = head;
  ListNode *second = head;

  // 将第一个指针移动到第 k 个节点
  for (int i = 0; i < k; i++) {
    if (first == NULL) {
      printf("链表的长度小于 %d，查找失败！\n", k);
      return NULL;
    }
    first = first->next;
  }

  // 同时移动两个指针，直到第一个指针到达链表末尾
  while (first != NULL) {
    first = first->next;
    second = second->next;
  }

  return second;
}

int main() {
  ListNode *head = NULL;

  // 创建一个链表
  insert_head(&head, 10);
  insert_tail(&head, 20);
  insert_tail(&head, 30);
  insert_tail(&head, 40);
  insert_tail(&head, 50);

  // 显示链表
  printf("原链表内容：");
  display_list(head);

  // 找倒数第k个节点
  int k = 6;
  ListNode *kth_node = find_kth_from_end(head, k);
  if (kth_node != NULL) {
    printf("链表中倒数第 %d 个节点的值为：%d\n", k, kth_node->data);
  } else {
    printf("链表中不存在倒数第 %d 个节点\n", k);
  }

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}