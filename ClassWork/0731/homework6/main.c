/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/homework6/main.c
 * @brief        : 作业6：合并两个有序链表
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
 * @name: merge_sorted_lists
 * @brief: 合并两个有序链表
 * @param {ListNode*} list1 第一个有序链表的头指针
 * @param {ListNode*} list2 第二个有序链表的头指针
 * @return {ListNode*} 合并后的有序链表头指针
 * @date: 2024-07-31 16:37:22
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
ListNode *merge_sorted_lists(ListNode *list1, ListNode *list2) {
  // 检查链表是否为空
  if (list1 == NULL && list2 == NULL) {
    printf("提供的链表是NULL，合并失败！\n");
    return NULL;
  }

  // 创建一个虚拟头节点，以便于处理链表合并的边界情况
  ListNode dummy;          // 虚拟头节点
  ListNode *tail = &dummy; // 当前处理节点，初始化指向虚拟头节点
  dummy.next = NULL;       // 虚拟头节点的 next 指针初始化为空

  // 合并两个有序链表
  while (list1 != NULL && list2 != NULL) {
    if (list1->data <= list2->data) { // 比较两个链表当前节点的数据大小
      tail->next = list1;  // 将较小的节点连接到结果链表中
      list1 = list1->next; // 移动 list1 指针到下一个节点
    } else {
      tail->next = list2;  // 将较小的节点连接到结果链表中
      list2 = list2->next; // 移动 list2 指针到下一个节点
    }
    tail = tail->next; // 移动 tail 指针到新的链表末尾
  }

  // 连接剩余部分
  if (list1 != NULL) {  // 如果 list1 还有剩余节点
    tail->next = list1; // 直接连接到结果链表末尾
  } else {
    tail->next = list2; // 否则将 list2 剩余节点连接到结果链表末尾
  }

  return dummy.next; // 返回合并后的有序链表的头指针
}

int main() {
  ListNode *list1 = NULL;
  ListNode *list2 = NULL;

  // 创建第一个有序链表
  insert_tail(&list1, 1);
  insert_tail(&list1, 3);
  insert_tail(&list1, 5);
  insert_tail(&list1, 7);

  // 创建第二个有序链表
  insert_tail(&list2, 2);
  insert_tail(&list2, 4);
  insert_tail(&list2, 6);
  insert_tail(&list2, 8);

  // 显示两个原始链表
  printf("第一个有序链表：");
  display_list(list1);

  printf("第二个有序链表：");
  display_list(list2);

  // 合并两个有序链表
  ListNode *merged_list = merge_sorted_lists(list1, list2);

  // 显示合并后的链表
  printf("合并后的有序链表：");
  display_list(merged_list);

  // 清理链表
  while (merged_list != NULL) {
    delete_head(&merged_list);
  }

  return 0;
}