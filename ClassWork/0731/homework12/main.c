/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework12/main.c
 * @brief        : 作业12：判断一个链表是否有环，并返回环的入口点
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
 * @name: detect_cycle
 * @brief: 判断一个链表是否有环，并返回环的入口点
 * @param {ListNode*} head 链表的头节点指针
 * @return {ListNode*} 如果链表有环，返回环的入口点；否则返回 NULL
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note: 使用快慢指针法检测链表是否有环，并找到环的入口点。
 *****************************************************************************************************************************************************************/
ListNode *detect_cycle(ListNode *head) {
  // 使用快慢指针法检测环
  ListNode *slow = head; // 慢指针，每次移动一步
  ListNode *fast = head; // 快指针，每次移动两步

  // 遍历链表
  while (fast != NULL && fast->next != NULL) {
    slow = slow->next;       // 慢指针移动一步
    fast = fast->next->next; // 快指针移动两步

    // 如果快指针与慢指针相遇，说明链表有环
    if (slow == fast) {
      // 寻找环的入口点
      ListNode *entry = head;
      while (entry != slow) {
        entry = entry->next; // 入口指针每次移动一步
        slow = slow->next;   // 相遇指针每次移动一步
      }
      return entry; // 返回环的入口点
    }
  }

  // 如果快指针到达链表尾部，说明链表无环
  return NULL;
}

int main() {
  ListNode *head = NULL;

  // 创建一个链表
  insert_tail(&head, 1);
  insert_tail(&head, 2);
  insert_tail(&head, 3);
  insert_tail(&head, 4);
  insert_tail(&head, 5);

  // 创建环：将最后一个节点的 next 指针指向第二个节点
  ListNode *temp = head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = head->next;

  // 判断链表是否有环，并返回环的入口点
  ListNode *cycle_entry = detect_cycle(head);
  if (cycle_entry != NULL) {
    printf("链表有环，环的入口点值为：%d\n", cycle_entry->data);
  } else {
    printf("链表没有环\n");
  }

  // 为了防止死循环，手动断开环
  temp->next = NULL;

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}