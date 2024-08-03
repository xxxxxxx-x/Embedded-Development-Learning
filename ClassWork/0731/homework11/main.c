/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework11/main.c
 * @brief        : 作业11：判断一个链表是否有环
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
 * @name: has_cycle
 * @brief: 判断一个链表是否存在环
 * @param {ListNode*} head 链表的头节点指针
 * @return {bool} 如果链表存在环，返回 true；否则返回 false
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note: 使用快慢指针法检测链表是否有环。
 *****************************************************************************************************************************************************************/
bool has_cycle(ListNode *head) {
  // 使用快慢指针法
  ListNode *slow = head; // 慢指针，每次移动一步
  ListNode *fast = head; // 快指针，每次移动两步

  // 遍历链表
  while (fast != NULL && fast->next != NULL) {
    slow = slow->next;       // 慢指针移动一步
    fast = fast->next->next; // 快指针移动两步

    // 如果快指针与慢指针相遇，说明链表有环
    if (slow == fast) {
      return true;
    }
  }

  // 如果快指针到达链表尾部，说明链表无环
  return false;
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

  // 判断链表是否有环
  if (has_cycle(head)) {
    printf("链表有环\n");
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
