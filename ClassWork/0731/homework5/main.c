/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework5/main.c
 * @brief        : 作业5：找链表的中间结点
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
 * @name: find_middle_node
 * @brief: 查找链表的中间结点
 * @param {ListNode*} head 链表头指针
 * @return {ListNode*} 找到的中间结点指针，未找到返回 NULL
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
ListNode *find_middle_node(ListNode *head) {
  // 检查链表是否为空
  if (head == NULL) {
    printf("提供的链表是NULL，查找失败！\n");
    return NULL;
  }

  // 检查链表是否只有一个节点
  if (head->next == NULL) {
    printf("链表只有一个节点，即为中间节点\n");
    return head;
  }

  // 定义快慢指针，初始指向链表头节点
  ListNode *slow = head;
  ListNode *fast = head;

  // 使用快慢指针法查找中间节点
  // 快指针每次移动两步，慢指针每次移动一步
  // 当快指针到达链表末尾时，慢指针正好处于链表中间
  while (fast != NULL && fast->next != NULL) {
    slow = slow->next;       // 慢指针移动一步
    fast = fast->next->next; // 快指针移动两步
  }

  // 慢指针所指向的节点即为中间节点
  return slow;
}

int main() {
  ListNode *head = NULL;

  // 创建一个链表
  insert_head(&head, 10);
  insert_tail(&head, 20);
  insert_tail(&head, 30);
  insert_tail(&head, 30);
  insert_tail(&head, 40);
  insert_tail(&head, 50);

  // 显示链表
  printf("链表内容：");
  display_list(head);

  // 找到链表的中间节点
  ListNode *middle = find_middle_node(head);
  if (middle != NULL) {
    printf("链表的中间节点的值：");
    display_list(middle);
  } else {
    printf("链表为空，没有中间节点\n");
  }

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}