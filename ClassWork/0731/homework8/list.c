/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0730/homework1/list.c
 * @brief        : 单链表的实现文件，包含单链表的各个函数实现
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-30 16:21:20
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************************************************************************
 * @name: create_node
 * @brief: 创建一个新的节点
 * @param {int} value 节点数据
 * @return {ListNode*} 新创建的节点指针
 * @date: 2024-08-01 13:13:51
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
ListNode *create_node(int value) {
  ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));  // 为新节点分配内存
  if (!new_node) {  // 检查内存分配是否成功
    printf("内存分配失败\n");
    exit(EXIT_FAILURE);  // 退出程序
  }
  new_node->data = value;  // 设置新节点的数据
  new_node->next = NULL;  // 设置新节点的 next 指针为 NULL
  return new_node;  // 返回新节点的指针
}

/****************************************************************************************************************************************************************
 * @name: insert_head
 * @brief: 在链表头部插入一个新节点
 * @param {ListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 13:14:18
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
bool insert_head(ListNode **head, int value) {
  ListNode *new_node = create_node(value);  // 创建新节点
  if (!new_node) {  // 检查新节点是否创建成功
    return false;
  }
  new_node->next = *head;  // 将新节点的 next 指针指向当前头节点
  *head = new_node;  // 将头指针指向新节点
  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_tail
 * @brief: 在链表尾部插入一个新节点
 * @param {ListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 13:14:26
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
bool insert_tail(ListNode **head, int value) {
  ListNode *new_node = create_node(value);  // 创建新节点
  if (!new_node) {  // 检查新节点是否创建成功
    return false;
  }
  if (*head == NULL) {  // 如果链表为空，将新节点作为头节点
    *head = new_node;
  } else {
    ListNode *temp = *head;
    while (temp->next) {  // 遍历到链表的最后一个节点
      temp = temp->next;
    }
    temp->next = new_node;  // 将最后一个节点的 next 指针指向新节点
  }
  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_at
 * @brief: 在链表的指定位置插入一个新节点
 * @param {ListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @param {int} position 插入位置
 * @return {void}
 * @date: 2024-08-01 13:14:32
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void insert_at(ListNode **head, int value, int position) {
  if (position == 0) {  // 如果插入位置是头部
    insert_head(head, value);
    return;
  }
  ListNode *new_node = create_node(value);  // 创建新节点
  if (!new_node) {  // 检查新节点是否创建成功
    return;
  }
  ListNode *temp = *head;
  for (int i = 0; i < position - 1 && temp; i++) {  // 遍历到指定位置的前一个节点
    temp = temp->next;
  }
  if (temp) {  // 如果指定位置有效
    new_node->next = temp->next;  // 将新节点的 next 指针指向原位置的后继节点
    temp->next = new_node;  // 将原前驱节点的 next 指针指向新节点
  } else {
    printf("位置超出范围\n");
    free(new_node);  // 释放新节点
  }
}

/****************************************************************************************************************************************************************
 * @name: delete_head
 * @brief: 删除链表的头节点
 * @param {ListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 13:14:41
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void delete_head(ListNode **head) {
  if (*head == NULL) {  // 检查链表是否为空
    printf("链表为空\n");
    return;
  }
  ListNode *temp = *head;
  *head = (*head)->next;  // 将头指针指向原头节点的后继节点
  free(temp);  // 释放原头节点
}

/****************************************************************************************************************************************************************
 * @name: delete_tail
 * @brief: 删除链表的尾节点
 * @param {ListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 13:14:48
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void delete_tail(ListNode **head) {
  if (*head == NULL) {  // 检查链表是否为空
    printf("链表为空\n");
    return;
  }
  ListNode *temp = *head;
  if (temp->next == NULL) {  // 如果链表只有一个节点
    free(temp);  // 释放唯一的节点
    *head = NULL;  // 将头指针设为 NULL
    return;
  }
  while (temp->next->next) {  // 遍历到倒数第二个节点
    temp = temp->next;
  }
  free(temp->next);  // 释放最后一个节点
  temp->next = NULL;  // 将倒数第二个节点的 next 指针设为 NULL
}

/****************************************************************************************************************************************************************
 * @name: delete_at
 * @brief: 删除链表指定位置的节点
 * @param {ListNode**} head 链表头指针的指针
 * @param {int} position 删除的位置
 * @return {void}
 * @date: 2024-08-01 13:14:55
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void delete_at(ListNode **head, int position) {
  if (*head == NULL) {  // 检查链表是否为空
    printf("链表为空\n");
    return;
  }
  if (position == 0) {  // 如果删除的位置是头节点
    delete_head(head);
    return;
  }
  ListNode *temp = *head;
  for (int i = 0; i < position - 1 && temp->next; i++) {  // 遍历到指定位置的前一个节点
    temp = temp->next;
  }
  if (temp->next) {  // 如果指定位置有效
    ListNode *to_delete = temp->next;  // 记录要删除的节点
    temp->next = temp->next->next;  // 将前一个节点的 next 指针指向要删除节点的后继节点
    free(to_delete);  // 释放要删除的节点
  } else {
    printf("位置超出范围\n");
  }
}

/****************************************************************************************************************************************************************
 * @name: update_value
 * @brief: 更新链表指定位置的节点数据
 * @param {ListNode*} head 链表头指针
 * @param {int} position 更新的位置
 * @param {int} new_value 新数据
 * @return {void}
 * @date: 2024-08-01 13:15:02
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void update_value(ListNode *head, int position, int new_value) {
  ListNode *temp = head;
  for (int i = 0; i < position && temp; i++) {  // 遍历到指定位置的节点
    temp = temp->next;
  }
  if (temp) {  // 如果指定位置有效
    temp->data = new_value;  // 更新节点的数据
  } else {
    printf("位置超出范围\n");
  }
}

/****************************************************************************************************************************************************************
 * @name: search_value
 * @brief: 搜索链表中第一个匹配指定数据的节点
 * @param {ListNode*} head 链表头指针
 * @param {int} value 搜索的数据
 * @return {ListNode*} 找到的节点指针，未找到返回 NULL
 * @date: 2024-08-01 13:15:10
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
ListNode *search_value(ListNode *head, int value) {
  ListNode *temp = head;
  while (temp) {  // 遍历链表
    if (temp->data == value) {  // 如果找到匹配的数据
      return temp;  // 返回该节点的指针
    }
    temp = temp->next;  // 移动到下一个节点
  }
  return NULL;  // 如果未找到，返回 NULL
}

/****************************************************************************************************************************************************************
 * @name: display_list
 * @brief: 打印链表中的所有节点数据
 * @param {ListNode*} head 链表头指针
 * @return {void}
 * @date: 2024-08-01 13:15:17
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void display_list(ListNode *head) {
  ListNode *temp = head;
  while (temp) {  // 遍历链表
    printf("%d -> ", temp->data);  // 打印节点的数据
    temp = temp->next;  // 移动到下一个节点
  }
  printf("NULL\n");  // 打印链表结束标志
}
