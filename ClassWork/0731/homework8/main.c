/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework8/main.c
 * @brief        : 作业8：删除排序链表中的重复元素
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
 * @name: delete_duplicates
 * @brief: 删除排序链表中的重复元素，只保留原始元素
 * @param {ListNode*} head 链表头指针
 * @return {*}
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
void delete_duplicates(ListNode *head) {
  // 检查链表是否为空
  if (head == NULL) {
    printf("提供的链表是NULL, 去重失败！\n");
    exit(1);
  }

  ListNode *current = head;
  while (current != NULL && current->next != NULL) {
    // 检查当前节点和下一个节点的数据是否相同
    if (current->data == current->next->data) {
      ListNode *duplicate = current->next;
      current->next = current->next->next; // 跳过重复节点
      free(duplicate);                     // 释放重复节点的内存
    } else {
      current = current->next; // 移动到下一个节点
    }
  }
}

int main() {
  ListNode *head = NULL;

  // 创建一个排序链表
  insert_tail(&head, 1);
  insert_tail(&head, 2);
  insert_tail(&head, 2);
  insert_tail(&head, 3);
  insert_tail(&head, 3);
  insert_tail(&head, 3);
  insert_tail(&head, 3);
  insert_tail(&head, 3);
  insert_tail(&head, 3);
  insert_tail(&head, 4);

  // 显示原始链表
  printf("原链表内容：");
  display_list(head);

  // 删除重复元素
  delete_duplicates(head);

  // 显示删除重复元素后的链表
  printf("删除重复元素后的链表内容：");
  display_list(head);

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}