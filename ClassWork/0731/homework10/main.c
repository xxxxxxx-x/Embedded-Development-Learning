/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework10/main.c
 * @brief        : 作业10：找到两个相交链表的第一个公共节点
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
 * @name: get_intersection_node
 * @brief: 找到两个链表中第一个相交的节点
 * @param {ListNode*} headA 链表A的头节点指针
 * @param {ListNode*} headB 链表B的头节点指针
 * @return {ListNode*} 第一个相交节点的指针；如果两个链表没有相交，则返回 NULL
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *如果两个链表存在公共节点，则返回第一个公共节点的指针；如果不存在公共节点，则返回
 *NULL。
 *****************************************************************************************************************************************************************/
ListNode *get_intersection_node(ListNode *headA, ListNode *headB) {
  // 检查链表是否为空
  if (headA == NULL || headB == NULL) {
    printf("提供的链表是NULL,判断失败！\n");
    return NULL; // 返回NULL而不是退出程序
  }

  // 遍历链表A的每一个节点
  ListNode *currentA = headA;
  while (currentA) {
    // 遍历链表B的每一个节点
    ListNode *currentB = headB;
    while (currentB) {
      // 如果找到了公共节点，则返回该节点
      if (currentA == currentB) {
        return currentA;
      }
      currentB = currentB->next;
    }
    currentA = currentA->next;
  }

  // 如果遍历结束没有找到公共节点，则返回NULL
  return NULL;
}

int main() {
  ListNode *headA = NULL;
  ListNode *headB = NULL;

  // 创建第一个链表
  insert_tail(&headA, 1);
  insert_tail(&headA, 2);

  // 创建第二个链表
  insert_tail(&headB, 9);
  insert_tail(&headB, 8);

  // 创建公共部分
  ListNode *common = create_node(3);
  insert_tail(&common, 4);
  insert_tail(&common, 5);

  // 连接两个链表到公共部分
  ListNode *temp = headA;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = common;

  temp = headB;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = common;

  // 显示两个链表
  printf("链表A：");
  display_list(headA);

  printf("链表B：");
  display_list(headB);

  // 找到两个相交链表的第一个公共节点
  ListNode *intersection = get_intersection_node(headA, headB);
  if (intersection != NULL) {
    printf("两个链表的第一个公共节点的值为：%d\n", intersection->data);
  } else {
    printf("两个链表没有公共节点\n");
  }

  // 清理链表
  while (headA != common) {
    delete_head(&headA);
  }
  while (headB != common) {
    delete_head(&headB);
  }
  while (common != NULL) {
    delete_head(&common);
  }

  return 0;
}