/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework2/main.c
 * @brief        : 作业2：删除链表中等于给定值 val 的所有节点
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-31 16:37:22
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************************************************************************
 * @name: delete_all_nodes_with_value
 * @brief: 删除链表中所有值等于给定值的节点
 * @param {ListNode**} head 链表头指针的指针
 * @param {int} value 要删除的节点的值
 * @return {bool} 删除成功返回 true
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
bool delete_all_nodes_with_value(ListNode **head, int value) {
  // 检查链表是否为空
  if (head == NULL || *head == NULL) {
    printf("提供的链表是NULL，删除指定值的所有节点失败！\n");
    return false;
  }

  ListNode *current = *head; // 当前节点指针
  ListNode *prev = NULL;     // 前一个节点指针

  while (current != NULL) {
    if (current->data == value) { // 如果当前节点的值等于要删除的值
      ListNode *to_delete = current; // 暂存当前节点
      if (current == *head) {        // 如果要删除的是头节点
        *head = current->next;       // 更新头节点
      } else {
        prev->next = current->next; // 更新前一个节点的 next 指针
      }
      current = current->next; // 移动到下一个节点
      free(to_delete);         // 释放当前节点的内存
    } else {
      prev = current;          // 更新前一个节点
      current = current->next; // 移动到下一个节点
    }
  }

  return true;
}

int main() {
  ListNode *head = NULL;

  // 创建一个链表
  insert_head(&head, 10);
  insert_head(&head, 10);
  insert_tail(&head, 20);
  insert_tail(&head, 10);
  insert_tail(&head, 10);
  insert_tail(&head, 30);
  insert_tail(&head, 10);
  insert_tail(&head, 10);

  // 显示链表
  printf("链表内容：");
  display_list(head);

  // 删除所有值为10的节点
  int val_to_delete = 10;
  delete_all_nodes_with_value(&head, val_to_delete);

  // 显示链表
  printf("删除所有值为 %d 的节点后的链表内容：", val_to_delete);
  display_list(head);

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}