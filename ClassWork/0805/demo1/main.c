/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0805/demo1/main.c
 * @brief        : 小练习：链表操作示例，包括节点的插入、遍历、删除、查找和修改
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-05 10:14:07
 * @version      : 1.0
 * @note         : 示例代码展示了如何使用内核链表进行基本的链表操作
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
// 添加内核链表头文件
#include "kernel_list.h"

/****************************************************************************************************************************************************************
 * @name:        struct Node
 * @brief:       定义链表节点结构体
 * @param {int} data: 节点存储的数据
 * @param {list_head} list: 内核链表节点，用于连接链表
 * @return {*}
 * @date:        2024-08-05 14:30:41
 * @version:     1.0
 * @note:        包含数据和链表指针
 *****************************************************************************************************************************************************************/
struct Node {
  int data;              // 节点数据
  struct list_head list; // 链表指针，用于连接其他节点
};

/****************************************************************************************************************************************************************
 * @name:        create_node
 * @brief:       创建并初始化一个新的节点
 * @param {int} data: 节点要存储的数据
 * @return:       新创建的节点指针
 * @date:        2024-08-05 14:30:49
 * @version:     1.0
 * @note:        分配内存并初始化链表节点
 *****************************************************************************************************************************************************************/
struct Node *create_node(int data) {
  // 分配内存，创建新的节点
  struct Node *new_node = malloc(sizeof(struct Node));
  if (!new_node) {
    // 如果内存分配失败，打印错误信息并返回 NULL
    printf("内存分配失败\n");
    return NULL;
  }
  new_node->data = data;           // 设置节点数据
  INIT_LIST_HEAD(&new_node->list); // 初始化链表节点

  return new_node; // 返回新创建的节点指针
}

/****************************************************************************************************************************************************************
 * @name:        insert_node
 * @brief:       将新节点插入到链表的尾部
 * @param {list_head} *head: 链表头指针
 * @param {int} data: 节点要存储的数据
 * @return:       无
 * @date:        2024-08-05 14:30:55
 * @version:     1.0
 * @note:        创建节点并将其添加到链表尾部
 *****************************************************************************************************************************************************************/
void insert_node(struct list_head *head, int data) {
  // 创建新节点
  struct Node *new_node = create_node(data);
  if (!new_node)
    return; // 如果节点创建失败，退出函数

  // 将新节点添加到链表的尾部
  list_add_tail(&new_node->list, head);
}

/****************************************************************************************************************************************************************
 * @name:        show_nodes
 * @brief:       遍历并显示链表中的所有节点
 * @param {list_head} *head: 链表头指针
 * @return:       无
 * @date:        2024-08-05 14:31:12
 * @version:     1.0
 * @note:        遍历链表并打印每个节点的数据
 *****************************************************************************************************************************************************************/
void show_nodes(struct list_head *head) {
  struct list_head *pos;
  // 遍历链表
  LIST_FOR_EACH(pos, head) {
    // 根据链表节点指针获取包含数据的结构体指针
    struct Node *node = LIST_ENTRY(pos, struct Node, list);
    printf("%d ", node->data); // 打印节点数据
  }
  printf("\n"); // 打印换行符
}

/****************************************************************************************************************************************************************
 * @name:        delete_node
 * @brief:       从链表中删除指定数据的节点
 * @param {list_head} *head: 链表头指针
 * @param {int} del_data: 要删除的节点数据
 * @return:       无
 * @date:        2024-08-05 14:31:21
 * @version:     1.0
 * @note:        遍历链表找到并删除指定数据的节点
 *****************************************************************************************************************************************************************/
void delete_node(struct list_head *head, int del_data) {
  struct list_head *pos, *n;
  // 遍历链表
  LIST_FOR_EACH_SAFE(pos, n, head) {
    // 根据链表节点指针获取包含数据的结构体指针
    struct Node *node = LIST_ENTRY(pos, struct Node, list);

    // 如果找到匹配的数据，删除该节点
    if (node->data == del_data) {
      list_del(pos); // 从链表中删除节点
      free(node);    // 释放节点内存
      return;        // 删除成功后退出函数
    }
  }
}

/****************************************************************************************************************************************************************
 * @name:        find_node
 * @brief:       查找链表中包含指定数据的节点
 * @param {list_head} *head: 链表头指针
 * @param {int} search_data: 要查找的数据
 * @return:       包含指定数据的节点指针，如果未找到返回 NULL
 * @date:        2024-08-05 14:31:29
 * @version:     1.0
 * @note:        遍历链表查找并返回匹配的数据节点
 *****************************************************************************************************************************************************************/
struct Node *find_node(struct list_head *head, int search_data) {
  struct list_head *pos;
  // 遍历链表
  LIST_FOR_EACH(pos, head) {
    // 根据链表节点指针获取包含数据的结构体指针
    struct Node *node = LIST_ENTRY(pos, struct Node, list);

    // 如果找到匹配的数据，返回节点指针
    if (node->data == search_data) {
      return node;
    }
  }

  return NULL; // 如果未找到，返回 NULL
}

/****************************************************************************************************************************************************************
 * @name:        modify_node
 * @brief:       修改链表中指定数据节点的值
 * @param {list_head} *head: 链表头指针
 * @param {int} old_data: 要修改的节点的旧数据
 * @param {int} new_data: 新的数据
 * @return:       无
 * @date:        2024-08-05 14:31:36
 * @version:     1.0
 * @note:        查找节点并修改其数据值
 *****************************************************************************************************************************************************************/
void modify_node(struct list_head *head, int old_data, int new_data) {
  // 查找包含旧数据的节点
  struct Node *node = find_node(head, old_data);
  if (node == NULL) {
    printf("数据未找到\n"); // 如果未找到，打印错误信息
  } else {
    node->data = new_data; // 修改节点数据
  }
}

/****************************************************************************************************************************************************************
 * @name:        list_splice_tail
 * @brief:       将源链表的节点拼接到目标链表的尾部
 * @param {list_head} *source: 源链表
 * @param {list_head} *target: 目标链表
 * @return:       无
 * @date:        2024-08-05 14:31:43
 * @version:     1.0
 * @note:        将源链表中的所有节点移动到目标链表的尾部
 *****************************************************************************************************************************************************************/
void list_splice_tail(struct list_head *source, struct list_head *target) {
  if (list_empty(source)) // 如果源链表为空，直接返回
    return;

  struct list_head *first = source->next; // 源链表第一个节点
  struct list_head *last = source->prev;  // 源链表最后一个节点

  // 更新目标链表的链接指针，将源链表拼接到目标链表
  target->prev->next = first;
  first->prev = target->prev;
  last->next = target;
  target->prev = last;

  // 初始化源链表为空链表
  INIT_LIST_HEAD(source);
}

int main() {
  // 创建链表头节点
  struct list_head head;
  INIT_LIST_HEAD(&head); // 初始化链表头节点

  // 向链表中插入节点
  insert_node(&head, 1);
  insert_node(&head, 2);
  insert_node(&head, 3);
  insert_node(&head, 4);
  insert_node(&head, 5);
  insert_node(&head, 6);
  insert_node(&head, 7);
  insert_node(&head, 8);
  insert_node(&head, 9);
  insert_node(&head, 0);

  // 显示链表中的所有节点
  printf("链表内容: ");
  show_nodes(&head);

  // 创建临时链表
  struct list_head temp_list;
  INIT_LIST_HEAD(&temp_list); // 初始化临时链表

  // 遍历链表，将偶数节点移动到临时链表
  struct list_head *pos, *n;
  LIST_FOR_EACH_SAFE(pos, n, &head) {
    struct Node *node = LIST_ENTRY(pos, struct Node, list);
    if (node->data % 2 == 0) {
      list_del(pos);                          // 从链表中删除偶数节点
      list_add_tail(&node->list, &temp_list); // 将偶数节点添加到临时链表
    }
  }

  // 将临时链表中的节点移动到原链表的尾部
  list_splice_tail(&temp_list, &head);

  // 显示修改后的链表
  printf("修改后链表内容: ");
  show_nodes(&head);

  return 0; // 程序正常结束
}
