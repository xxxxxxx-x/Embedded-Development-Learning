/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0801/homework2/dclist.c
 * @brief        :
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-01 17:28:20
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "dclist.h"
#include <stdbool.h>
#include <stdio.h>

/****************************************************************************************************************************************************************
 * @name: create_dcnode
 * @brief: 创建一个新的双链表节点
 * @param {int} value 节点数据
 * @return {DCListNode*} 新创建的节点指针
 * @date: 2024-08-01 16:14:59
 * @version: 1.0
 * @note: 如果内存分配失败，将打印错误信息并退出程序
 *****************************************************************************************************************************************************************/
DCListNode *create_dcnode(int value) {
  DCListNode *new_node = malloc(sizeof(DCListNode));
  if (new_node == NULL) {
    perror("创建节点失败");
    exit(EXIT_FAILURE);
  }

  new_node->data = value;
  new_node->prev = new_node;
  new_node->next = new_node;

  return new_node;
}

/****************************************************************************************************************************************************************
 * @name: insert_head
 * @brief: 在双链表的头部插入一个新节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 16:15:12
 * @version: 1.0
 * @note: 如果链表为空，新节点将成为头节点；否则，将新节点插入到链表头部
 *****************************************************************************************************************************************************************/
bool insert_dcnode_head(DCListNode **head, int value) {
  if (head == NULL)
    return false;

  DCListNode *new_node = create_dcnode(value);
  if (*head == NULL) {
    *head = new_node;
  } else {
    // 版本1：头插法保留原头节点，在原头节点之后插入新节点
    // new_node->next = (*head)->next;
    // new_node->prev = (*head);
    // (*head)->next = new_node;
    // if ((*head)->prev == *(head)) {
    //   (*head)->prev = new_node;
    // } else {
    //   (*head)->next->next->prev = new_node;
    // }

    // 版本2：头插法不保留原头节点，新节点作为新的头节点
    new_node->prev = (*head)->prev;
    new_node->next = (*head);
    (*head)->prev = new_node;
    new_node->prev->next = new_node;
    *head = new_node;
  }

  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_tail
 * @brief: 在双链表的尾部插入一个新节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 16:15:19
 * @version: 1.0
 * @note:
 *如果链表为空，新节点将成为头节点；否则，遍历链表并将新节点插入到链表尾部
 *****************************************************************************************************************************************************************/
bool insert_dcnode_tail(DCListNode **head, int value) {
  if (head == NULL)
    return false;

  DCListNode *new_node = create_dcnode(value);
  if (*head == NULL) {
    *head = new_node;
  } else {
    new_node->next = (*head);
    if ((*head)->next == (*head) && (*head)->prev == (*head)) {
      new_node->prev = (*head);
      (*head)->next = new_node;
      (*head)->prev = new_node;
    } else {
      new_node->prev = (*head)->prev;
      new_node->prev->next = new_node;
      (*head)->prev = new_node;
    }
  }

  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_at
 * @brief: 在双链表的指定位置插入一个新节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @param {int} position 插入位置
 * @return {void}
 * @date: 2024-08-01 16:15:26
 * @version: 1.0
 * @note: 如果位置为 0，将调用 `insert_head`
 *函数；否则，遍历链表并将新节点插入到指定位置
 *****************************************************************************************************************************************************************/
void insert_dcnode_at(DCListNode **head, int value, int position) {

  if (head == NULL || position < 0)
    return;

  if (position == 0 || position == 1) {
    // 在链表头部插入新节点
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
      printf("插入失败：位置超出链表范围");
      free(new_node);
      return;
    }
  }

  // 插入新节点
  new_node->next = temp->next;
  new_node->prev = temp;
  new_node->next->prev = new_node;
  temp->next = new_node;
}

/****************************************************************************************************************************************************************
 * @name: delete_head
 * @brief: 删除双链表的头节点
 * @param {DCListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 16:15:34
 * @version: 1.0
 * @note: 如果链表为空或头节点为空，什么都不做；否则，删除头节点并更新头指针
 *****************************************************************************************************************************************************************/
void delete_dcnode_head(DCListNode **head) {
  if (head == NULL || *head == NULL)
    return;

  // 保存头节点
  DCListNode *temp = *head;
  // 更新头指针
  *head = (*head)->next;
  if (*head != temp) {
    // 更新新的头节点的前驱指针
    (*head)->prev = temp->prev;
    temp->prev->next = (*head);
  } else {
    *head = NULL;
  }
  // 释放删除的节点
  free(temp);
}

/****************************************************************************************************************************************************************
 * @name: delete_tail
 * @brief: 删除双链表的尾节点
 * @param {DCListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 16:15:40
 * @version: 1.0
 * @note: 如果链表为空，什么都不做；如果链表只有一个节点，删除后将头指针设为
 *NULL；否则，遍历链表找到尾节点并删除
 *****************************************************************************************************************************************************************/
void delete_dcnode_tail(DCListNode **head) {
  if (head == NULL || *head == NULL)
    return;

  if ((*head)->next == *head) {
    // 如果链表只有一个节点，删除后将头指针设为 NULL
    *head = NULL;
    free(*head);
    return;
  }

  DCListNode *to_del = (*head)->prev;
  // 删除尾节点
  to_del->prev->next = (*head);
  (*head)->prev = to_del->prev;
  free(to_del);
}

/****************************************************************************************************************************************************************
 * @name: delete_at
 * @brief: 删除双链表指定位置的节点
 * @param {DCListNode**} head 链表头指针的指针
 * @param {int} position 删除的位置
 * @return {void}
 * @date: 2024-08-01 16:15:48
 * @version: 1.0
 * @note: 如果位置为 0，将调用 `delete_head`
 *函数；否则，遍历链表找到指定位置的节点并删除
 *****************************************************************************************************************************************************************/
void delete_dcnode_at(DCListNode **head, int position) {
  if (head == NULL || *head == NULL || position < 0)
    return;

  if (position == 0 || position == 1) {
    // 删除头节点
    delete_dcnode_head(head);
    return;
  }

  DCListNode *to_del_prev = *head;

  // 遍历链表找到指定位置的节点
  for (int i = 0; i < position - 2; i++) {
    to_del_prev = to_del_prev->next;
    if (to_del_prev == *head) {
      printf("\n删除失败：位置超出链表范围");
      return;
    }
  }

  DCListNode *to_del = to_del_prev->next;

  // 删除节点
  to_del_prev->next = to_del->next;
  to_del->next->prev = to_del_prev;

  // 释放删除的节点
  free(to_del);
}

/****************************************************************************************************************************************************************
 * @name: update_value
 * @brief: 更新双链表指定位置节点的数据
 * @param {DCListNode*} head 链表头指针
 * @param {int} position 更新位置
 * @param {int} new_value 新数据值
 * @return {void}
 * @date: 2024-08-01 16:15:56
 * @version: 1.0
 * @note:
 *遍历链表找到指定位置的节点并更新其数据值；如果位置超出链表范围，打印错误信息
 *****************************************************************************************************************************************************************/
void update_dcnode_value(DCListNode *head, int position, int new_value) {

  if (head == NULL || position < 0)
    return;

  // 遍历链表找到指定位置的节点
  DCListNode *temp = head;
  for (int i = 0; i < position - 1; i++) {
    temp = temp->next;
    if (temp == head) {
      temp = NULL;
    }
  }

  if (temp == NULL) {
    // 如果位置超出链表范围，打印错误信息
    printf("更新失败：位置超出链表范围\n");
    return;
  }

  // 更新节点数据
  temp->data = new_value;
}

/****************************************************************************************************************************************************************
 * @name: search_value
 * @brief: 在双链表中查找指定值的节点
 * @param {DCListNode*} head 链表头指针
 * @param {int} value 要查找的值
 * @return {DCListNode*} 查找到的节点指针；如果未找到，则返回 NULL
 * @date: 2024-08-01 16:16:03
 * @version: 1.0
 * @note: 遍历链表查找指定值的节点
 *****************************************************************************************************************************************************************/
DCListNode *search_dcnode_value(DCListNode *head, int value) {
  DCListNode *dest_node = head;
  while (dest_node != NULL) {
    if (dest_node->data == value) {
      return dest_node;
    }
    dest_node = dest_node->next;
    if (dest_node == head) {
      printf("\n查找失败：位置超出链表范围");
      return NULL;
    }
  }

  return false;
}

/****************************************************************************************************************************************************************
 * @name: display_list
 * @brief: 打印双链表中的所有节点数据
 * @param {DCListNode*} head 链表头指针
 * @return {void}
 * @date: 2024-08-01 16:16:14
 * @version: 1.0
 * @note: 遍历链表并打印节点数据，最后打印换行符
 *****************************************************************************************************************************************************************/
void display_dlist(DCListNode *head) {

  DCListNode *temp = head;

  printf("\n后遍历：");

  while (temp != NULL) {
    printf("%d ", temp->data);
    temp = temp->next;
    if (temp == head) {
      break;
    }
  }

  printf("\n前遍历：");

  temp = head;

  while (temp != NULL) {
    temp = temp->prev;
    printf("%d ", temp->data);
    if (temp == head) {
      break;
    }
  }

  printf("\n");
}
