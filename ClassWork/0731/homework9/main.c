/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework9/main.c
 * @brief        : 作业9：判断一个链表是否是回文链表
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
 * @name: is_palindrome
 * @brief: 判断链表是否是回文链表
 * @param {ListNode*} head 链表的头节点指针
 * @return {bool} 如果链表是回文链表，返回 true；否则返回 false
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
bool is_palindrome(ListNode *head) {
  // 检查链表是否为空
  if (head == NULL) {
    printf("提供的链表是NULL,判断失败！\n");
    return false;
  }

  // 计算链表的长度
  unsigned int list_len = 0;
  ListNode *temp = head;
  while (temp) {
    list_len++;
    temp = temp->next;
  }

  // 使用两个指针分别遍历链表的前半部分和后半部分
  ListNode *first_half = head;
  ListNode *second_half = NULL;
  ListNode *prev = NULL;

  // 找到链表的中间节点，并将后半部分反转
  temp = head;
  for (unsigned int i = 0; i < list_len / 2; i++) {
    prev = temp;
    temp = temp->next;
  }
  if (list_len % 2 != 0) {
    temp = temp->next; // 跳过中间节点（如果链表长度是奇数）
  }
  prev->next = NULL; // 切断链表前半部分
  second_half = temp;

  // 反转链表的后半部分
  ListNode *reversed_second_half = NULL;
  while (second_half) {
    ListNode *next_node = second_half->next;
    second_half->next = reversed_second_half;
    reversed_second_half = second_half;
    second_half = next_node;
  }

  // 比较前半部分和后半部分
  ListNode *first = first_half;
  ListNode *second = reversed_second_half;
  while (first && second) {
    if (first->data != second->data) {
      return false;
    }
    first = first->next;
    second = second->next;
  }

  // 如果链表的前半部分和反转后的后半部分都比较完且无不匹配的项，则为回文链表
  return true;
}

int main() {
  ListNode *head = NULL;

  // 创建一个链表
  insert_tail(&head, 1);
  insert_tail(&head, 2);
  insert_tail(&head, 3);
  insert_tail(&head, 3);
  insert_tail(&head, 1);

  // 显示原始链表
  printf("原链表内容：");
  display_list(head);

  // 判断链表是否是回文链表
  bool result = is_palindrome(head);
  if (result) {
    printf("链表是回文链表\n");
  } else {
    printf("链表不是回文链表\n");
  }

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}