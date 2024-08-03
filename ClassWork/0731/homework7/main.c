/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework7/main.c
 * @brief        : 作业7：分隔链表
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
 * @name: split_list
 * @brief:
 *分隔链表，将链表分成两部分，前半部分包含指定位置之前的节点，后半部分包含指定位置及之后的节点
 * @param {ListNode*} head 链表头指针
 * @param {int} position 指定位置，从1开始
 * @param {ListNode**} first_half 前半部分链表头指针
 * @param {ListNode**} second_half 后半部分链表头指针
 * @return {*}
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
void split_list(ListNode *head, int position, ListNode **first_half,
                ListNode **second_half) {
  // 检查链表是否为空
  if (head == NULL) {
    printf("提供的链表是NULL,分割失败！\n");
    exit(1);
  }

  // 初始化前半部分和后半部分链表头指针
  *first_half = malloc(sizeof(ListNode));
  *second_half = malloc(sizeof(ListNode));

  // 如果分隔位置是0，前半部分为空，后半部分为整个链表
  if (position == 0) {
    *first_half = NULL;
    *second_half = head;
    return;
  }

  // 遍历链表到指定位置
  ListNode *temp = head;
  *first_half = head;
  for (int i = 0; i < position - 1 && temp != NULL; i++) {
    temp = temp->next;
  }

  // 检查是否超出链表长度
  if (temp == NULL) {
    printf("分割失败,超出链表长度！\n");
    exit(1);
  }

  // 分割链表
  *second_half = temp->next;
  temp->next = NULL; // 将前半部分最后一个节点的next指针置为空
}

int main() {
  ListNode *head = NULL;
  ListNode *first_half = NULL;
  ListNode *second_half = NULL;

  // 创建一个链表
  insert_tail(&head, 1);
  insert_tail(&head, 2);
  insert_tail(&head, 3);
  insert_tail(&head, 4);
  insert_tail(&head, 5);

  // 显示原始链表
  printf("原链表内容：");
  display_list(head);

  // 分隔链表，在节点3处分割
  int position = 0; // 第三个节点的位置是索引2
  split_list(head, position, &first_half, &second_half);

  // 显示分隔后的两个链表
  printf("分隔后的第一个链表：");
  display_list(first_half);

  printf("分隔后的第二个链表：");
  display_list(second_half);

  // 清理链表
  while (first_half != NULL) {
    delete_head(&first_half);
  }
  while (second_half != NULL) {
    delete_head(&second_half);
  }

  return 0;
}