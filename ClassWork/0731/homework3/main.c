/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework3/main.c
 * @brief        : 作业3：反转单链表
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
 * @name: reverse_list
 * @brief: 反转单链表
 * @param {ListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
void reverse_list(ListNode **head) {
  // 检查链表是否为空
  if (head == NULL || *head == NULL) {
    printf("提供的链表是NULL，反转失败！\n");
    return;
  }

  ListNode *prev = NULL;     // 前一个节点指针
  ListNode *current = *head; // 当前节点指针
  ListNode *next = NULL;     // 下一个节点指针

  while (current != NULL) {
    next = current->next; // 保存下一个节点
    current->next = prev; // 当前节点的 next 指针指向前一个节点
    prev = current;       // 更新前一个节点指针
    current = next;       // 移动到下一个节点
  }

  *head = prev; // 更新头节点
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

  // 反转链表
  reverse_list(&head);

  // 显示反转后的链表
  printf("反转后的链表内容：");
  display_list(head);

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}