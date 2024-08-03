/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0801/homework2/dclist.c
 * @brief        : 双向循环链表的实现
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-01 17:28:20
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include "dclist.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************************************************************************
 * @name: create_dcnode
 * @brief: 创建一个新的双向循环链表节点
 * @param {int} value 节点数据
 * @return {DCListNode*} 新创建的节点指针
 * @date: 2024-08-01 16:14:59
 * @version: 1.0
 * @note: 如果内存分配失败，将打印错误信息并退出程序
 *****************************************************************************************************************************************************************/
DCListNode *create_dcnode(int value) {
  // 分配新节点内存
  DCListNode *new_node = (DCListNode *)malloc(sizeof(DCListNode));
  if (new_node == NULL) {
    perror("创建节点失败");
    exit(EXIT_FAILURE);
  }

  // 初始化节点数据和指针
  new_node->data = value;
  new_node->prev = new_node;
  new_node->next = new_node;

  return new_node;
}

/****************************************************************************************************************************************************************
 * @name: insert_dcnode_head
 * @brief: 在双向循环链表的头部插入一个新节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 16:15:12
 * @version: 1.0
 * @note: 如果链表为空，新节点将成为头节点；否则，将新节点插入到链表头部
 *****************************************************************************************************************************************************************/
bool insert_dcnode_head(DCListNode **head, int value) {
  // 检查头指针是否为空
  if (head == NULL)
    return false;

  // 创建新节点
  DCListNode *new_node = create_dcnode(value);

  // 如果链表为空，新节点成为头节点
  if (*head == NULL) {
    *head = new_node;
  } else {

    // // 版本1： 插入新节点到原头节点之后
    // new_node->next = (*head)->next;
    // new_node->prev = (*head);
    // (*head)->next = new_node;
    // new_node->next->prev = new_node;
    // // 如果链表只有一个节点，更新原头节点的前驱指针
    // if ((*head)->prev == *head) {
    //   (*head)->prev = new_node;
    // }

    // 版本2：头插法不保留原头节点，新节点作为新的头节点
    new_node->prev = (*head)->prev;
    new_node->next = (*head);
    (*head)->prev->next = new_node;
    (*head)->prev = new_node;
    *head = new_node;
  }

  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_dcnode_tail
 * @brief: 在双向循环链表的尾部插入一个新节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 16:15:19
 * @version: 1.0
 * @note:
 *如果链表为空，新节点将成为头节点；否则，遍历链表并将新节点插入到链表尾部
 *****************************************************************************************************************************************************************/
bool insert_dcnode_tail(DCListNode **head, int value) {
  // 检查头指针是否为空
  if (head == NULL)
    return false;

  // 创建新节点
  DCListNode *new_node = create_dcnode(value);

  // 如果链表为空，新节点成为头节点
  if (*head == NULL) {
    *head = new_node;
  } else {
    // 插入新节点到链表尾部
    new_node->next = (*head);
    new_node->prev = (*head)->prev;
    (*head)->prev->next = new_node;
    (*head)->prev = new_node;
  }

  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_dcnode_at
 * @brief: 在双向循环链表的指定位置插入一个新节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @param {int} position 插入位置
 * @return {void}
 * @date: 2024-08-01 16:15:26
 * @version: 1.0
 * @note: 如果位置为 0，将调用 `insert_dcnode_head`
 *函数；否则，遍历链表并将新节点插入到指定位置
 *****************************************************************************************************************************************************************/
void insert_dcnode_at(DCListNode **head, int value, int position) {
  // 检查头指针和位置的有效性
  if (head == NULL || position < 0)
    return;

  // 如果插入位置为0，调用头插函数
  if (position == 0) {
    insert_dcnode_head(head, value);
    return;
  }

  // 创建新节点
  DCListNode *new_node = create_dcnode(value);
  DCListNode *temp = *head;

  // 遍历链表找到插入位置的前一个节点
  for (int i = 0; i < position - 1; i++) {
    temp = temp->next;
    if (temp == *head) {
      // 如果位置超出链表范围，打印错误信息并释放内存
      printf("插入失败：位置超出链表范围\n");
      free(new_node);
      return;
    }
  }

  // 插入新节点
  new_node->next = temp->next;
  new_node->prev = temp;
  temp->next->prev = new_node;
  temp->next = new_node;
}

/****************************************************************************************************************************************************************
 * @name: delete_dcnode_head
 * @brief: 删除双向循环链表的头节点
 * @param {DCListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 16:15:34
 * @version: 1.0
 * @note: 如果链表为空或头节点为空，什么都不做；否则，删除头节点并更新头指针
 *****************************************************************************************************************************************************************/
void delete_dcnode_head(DCListNode **head) {
  // 检查头指针和头节点是否为空
  if (head == NULL || *head == NULL)
    return;

  // 保存头节点
  DCListNode *temp = *head;

  // 如果链表只有一个节点，删除后将头指针设为 NULL
  if ((*head)->next == *head) {
    *head = NULL;
  } else {
    // 更新头指针
    *head = (*head)->next;
    // 更新新的头节点的前驱指针
    (*head)->prev = temp->prev;
    temp->prev->next = (*head);
  }

  // 释放删除的节点
  free(temp);
}

/****************************************************************************************************************************************************************
 * @name: delete_dcnode_tail
 * @brief: 删除双向循环链表的尾节点
 * @param {DCListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 16:15:40
 * @version: 1.0
 * @note: 如果链表为空，什么都不做；如果链表只有一个节点，删除后将头指针设为
 *NULL；否则，遍历链表找到尾节点并删除
 *****************************************************************************************************************************************************************/
void delete_dcnode_tail(DCListNode **head) {
  // 检查头指针和头节点是否为空
  if (head == NULL || *head == NULL)
    return;

  // 如果链表只有一个节点，删除后将头指针设为 NULL
  if ((*head)->next == *head) {
    free(*head);
    *head = NULL;
    return;
  }

  // 找到尾节点
  DCListNode *tail_node = (*head)->prev;

  // 更新尾节点的前驱节点的指针
  tail_node->prev->next = *head;
  (*head)->prev = tail_node->prev;

  // 释放删除的尾节点
  free(tail_node);
}

/****************************************************************************************************************************************************************
 * @name: delete_dcnode_at
 * @brief: 删除双向循环链表指定位置的节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} position 删除的位置
 * @return {void}
 * @date: 2024-08-01 16:15:48
 * @version: 1.0
 * @note: 如果位置为 0，将调用 `delete_dcnode_head`
 *函数；否则，遍历链表找到指定位置的节点并删除
 *****************************************************************************************************************************************************************/
void delete_dcnode_at(DCListNode **head, int position) {
  // 检查头指针和头节点是否为空
  if (head == NULL || *head == NULL || position < 0)
    return;

  // 如果删除位置为0，调用头删除函数
  if (position == 0) {
    delete_dcnode_head(head);
    return;
  }

  DCListNode *temp = *head;

  // 遍历链表找到删除位置的节点
  for (int i = 0; i < position; i++) {
    temp = temp->next;
    if (temp == *head) {
      // 如果位置超出链表范围，打印错误信息
      printf("删除失败：位置超出链表范围\n");
      return;
    }
  }

  // 更新前驱和后继节点的指针
  temp->prev->next = temp->next;
  temp->next->prev = temp->prev;

  // 释放删除的节点
  free(temp);
}

/****************************************************************************************************************************************************************
 * @name: update_dcnode_value
 * @brief: 更新双向循环链表中第一个找到的指定值的节点数据
 * @param {DCListNode*} head 链表头指针
 * @param {int} old_value 旧的数据值
 * @param {int} new_value 新的数据值
 * @return {bool} 更新成功返回 true，否则返回 false
 * @date: 2024-08-01 16:16:03
 * @version: 1.0
 * @note: 遍历链表，找到第一个匹配的节点并更新其数据
 *****************************************************************************************************************************************************************/
bool update_dcnode_value(DCListNode *head, int old_value, int new_value) {
  if (head == NULL)
    return false;

  DCListNode *temp = head;

  // 遍历链表找到第一个匹配的节点并更新其数据
  do {
    if (temp->data == old_value) {
      temp->data = new_value;
      return true;
    }
    temp = temp->next;
  } while (temp != head);

  return false;
}

/****************************************************************************************************************************************************************
 * @name: search_dcnode_value
 * @brief: 在双向循环链表中搜索指定值的节点
 * @param {DCListNode*} head 链表头指针
 * @param {int} value 要搜索的值
 * @return {DCListNode*} 找到的节点指针，如果未找到返回 NULL
 * @date: 2024-08-01 16:16:13
 * @version: 1.0
 * @note: 遍历链表，找到第一个匹配的节点并返回其指针
 *****************************************************************************************************************************************************************/
DCListNode *search_dcnode_value(DCListNode *head, int value) {
  if (head == NULL)
    return NULL;

  DCListNode *temp = head;

  // 遍历链表找到第一个匹配的节点并返回其指针
  do {
    if (temp->data == value) {
      return temp;
    }
    temp = temp->next;
  } while (temp != head);

  return NULL;
}

/****************************************************************************************************************************************************************
 * @name: print_dclist_forward
 * @brief: 正向打印双向循环链表的所有节点数据
 * @param {DCListNode*} head 链表头指针
 * @return {void}
 * @date: 2024-08-01 16:16:21
 * @version: 1.0
 * @note: 如果链表为空，将打印
 *"链表为空"；否则，正向遍历链表并打印每个节点的数据
 *****************************************************************************************************************************************************************/
void print_dclist_forward(DCListNode *head) {
  // 检查链表是否为空
  if (head == NULL) {
    printf("链表为空\n");
    return;
  }

  DCListNode *temp = head;

  // 正向遍历链表并打印每个节点的数据
  do {
    printf("%d ", temp->data);
    temp = temp->next;
  } while (temp != head);

  printf("\n");
}

/****************************************************************************************************************************************************************
 * @name: print_dclist_backward
 * @brief: 反向打印双向循环链表的所有节点数据
 * @param {DCListNode*} head 链表头指针
 * @return {void}
 * @date: 2024-08-01 16:16:28
 * @version: 1.0
 * @note: 如果链表为空，将打印
 *"链表为空"；否则，反向遍历链表并打印每个节点的数据
 *****************************************************************************************************************************************************************/
void print_dclist_backward(DCListNode *head) {
  // 检查链表是否为空
  if (head == NULL) {
    printf("链表为空\n");
    return;
  }

  DCListNode *temp = head->prev;

  // 反向遍历链表并打印每个节点的数据
  do {
    printf("%d ", temp->data);
    temp = temp->prev;
  } while (temp != head->prev);

  printf("\n");
}

/****************************************************************************************************************************************************************
 * @name: free_dclist
 * @brief: 释放双向循环链表的所有节点内存
 * @param {DCListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 16:16:01
 * @version: 1.0
 * @note: 遍历链表并释放每个节点的内存，最后将头指针设为 NULL
 *****************************************************************************************************************************************************************/
void free_dclist(DCListNode **head) {
  // 检查头指针和头节点是否为空
  if (head == NULL || *head == NULL)
    return;

  DCListNode *temp = *head;

  // 遍历链表并释放每个节点
  do {
    DCListNode *next = temp->next;
    free(temp);
    temp = next;
  } while (temp != *head);

  // 将头指针设为 NULL
  *head = NULL;
}
