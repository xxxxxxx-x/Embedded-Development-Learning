/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0731/demo1/main.c
 * @brief        : 小练习：设计双链表
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-31 10:31:34
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 双链表节点结构体定义
struct node {
  int value;            // 节点数据
  struct node *prev;    // 指向前驱节点的指针
  struct node *next;    // 指向后继节点的指针
};

/****************************************************************************************************************************************************************
 * @name: createNode
 * @brief: 创建一个新节点
 * @param {int} value 节点数据
 * @return {struct node*} 新节点的指针
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
struct node *createNode(int value) {
  struct node *newNode = malloc(sizeof(struct node));  // 为新节点分配内存
  if (newNode == NULL) {  // 检查内存分配是否成功
    printf("创建失败！\n");
    return NULL;
  }

  newNode->value = value;  // 初始化新节点的数据
  newNode->prev = NULL;    // 初始化新节点的前驱指针为 NULL
  newNode->next = NULL;    // 初始化新节点的后继指针为 NULL
  return newNode;  // 返回新节点的指针
}

/****************************************************************************************************************************************************************
 * @name: insert_head
 * @brief: 在链表头部插入一个新节点
 * @param {struct node*} head 链表头指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
bool insert_head(struct node *head, int value) {
  struct node *newNode = createNode(value);  // 创建新节点
  if (newNode == NULL) {  // 检查新节点是否创建成功
    printf("头插失败！\n");
    return false;
  }

  if (head == NULL) {  // 如果链表为空，直接将新节点作为头节点
    head = newNode;
  } else if (head->next == NULL) {  // 如果链表只有一个节点，将新节点插入到头节点之后
    head->next = newNode;
    newNode->prev = head;
  } else {  // 如果链表有多个节点，将新节点插入到头节点之后
    newNode->next = head->next;
    newNode->prev = head;
    head->next->prev = newNode;
    head->next = newNode;
  }

  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_tail
 * @brief: 在链表尾部插入一个新节点
 * @param {struct node*} head 链表头指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
bool insert_tail(struct node *head, int value) {
  struct node *newNode = createNode(value);  // 创建新节点
  if (newNode == NULL) {  // 检查新节点是否创建成功
    printf("尾插失败！\n");
    return false;
  }

  if (head == NULL) {  // 如果链表为空，直接将新节点作为头节点
    head = newNode;
    return true;
  }

  struct node *temp = head;

  // 遍历到链表的最后一个节点
  while (temp->next != NULL) {
    temp = temp->next;
  }

  newNode->prev = temp;  // 将新节点的前驱指针指向最后一个节点
  temp->next = newNode;  // 将最后一个节点的后继指针指向新节点

  return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_at
 * @brief: 在链表的指定位置插入一个新节点
 * @param {struct node*} head 链表头指针
 * @param {int} value 新节点的数据
 * @param {int} position 插入的位置
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
bool insert_at(struct node *head, int value, int position) {
  if (head == NULL) {  // 检查链表是否为空
    printf("列表为空，指定插入失败！\n");
    return false;
  }

  struct node *temp = head;

  // 遍历到指定位置的前一个节点
  for (int i = 0; i < position - 1 && temp; i++) {
    temp = temp->next;
  }

  if (temp) {  // 检查指定位置是否有效
    struct node *newNode = createNode(value);  // 创建新节点
    if (newNode == NULL) {
      return false;
    }
    newNode->next = temp->next;  // 将新节点的后继指针指向原位置的后继节点
    newNode->prev = temp;        // 将新节点的前驱指针指向原位置的前驱节点
    if (temp->next) {
      temp->next->prev = newNode;  // 更新原后继节点的前驱指针指向新节点
    }
    temp->next = newNode;  // 将原前驱节点的后继指针指向新节点
  } else {
    printf("指定插入失败，position超出链表范围！\n");
    return false;
  }

  return true;
}

/****************************************************************************************************************************************************************
 * @name: del_head
 * @brief: 删除链表的头节点
 * @param {struct node**} head 链表头指针的指针
 * @return {bool} 删除成功返回 true，否则返回 false
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
bool del_head(struct node **head) {
  if (head == NULL || *head == NULL) {  // 检查链表是否为空
    printf("删除失败，链表为空或head为NULL！\n");
    return false;
  }

  struct node *to_del = *head;

  if ((*head)->next == NULL) {  // 如果链表只有一个节点
    free(*head);  // 释放头节点
    *head = NULL; // 头节点指针设为NULL
  } else {
    *head = (*head)->next;   // 将头节点指针指向原头节点的后继节点
    (*head)->prev = NULL;    // 更新新头节点的前驱指针为NULL
    to_del->next = NULL;     // 清除原头节点的后继指针
    free(to_del);            // 释放原头节点
  }

  return true;
}

/****************************************************************************************************************************************************************
 * @name: show_next
 * @brief: 顺序打印链表中的所有节点数据
 * @param {struct node*} head 链表头指针
 * @return {void}
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void show_next(struct node *head) {
  if (head == NULL) {  // 检查链表是否为空
    printf("当前列表没有任何节点！\n");
    return;
  }

  struct node *temp = head;

  // 顺序遍历链表并打印节点数据
  while (temp != NULL) {
    printf("%d -> ", temp->value);
    temp = temp->next;
  }
  printf("NULL\n");  // 打印链表结尾
}

/****************************************************************************************************************************************************************
 * @name: show_prev
 * @brief: 逆序打印链表中的所有节点数据
 * @param {struct node*} head 链表头指针
 * @return {void}
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
void show_prev(struct node *head) {
  if (head == NULL) {  // 检查链表是否为空
    printf("当前列表没有任何节点！\n");
    return;
  }

  struct node *temp = head;

  // 遍历到链表的最后一个节点
  while (temp->next != NULL) {
    temp = temp->next;
  }

  // 逆序遍历链表并打印节点数据
  while (temp != NULL) {
    printf("%d -> ", temp->value);
    temp = temp->prev;
  }
  printf("NULL\n");  // 打印链表开头
}

/****************************************************************************************************************************************************************
 * @name: main
 * @brief: 主函数，测试双链表的各种操作
 * @param {int} argc 参数个数
 * @param {const char*[]} argv 参数列表
 * @return {int} 程序退出状态码
 * @date: 2024-07-31 10:31:34
 * @version: 1.0
 * @note: 
*****************************************************************************************************************************************************************/
int main(int argc, const char *argv[]) {
  struct node *head = createNode(1);  // 创建链表头节点
  insert_tail(head, 2);               // 在链表尾部插入节点
  insert_tail(head, 3);               // 在链表尾部插入节点
  insert_tail(head, 4);               // 在链表尾部插入节点
  insert_tail(head, 5);               // 在链表尾部插入节点
  insert_at(head, -3, 3);             // 在链表指定位置插入节点
  show_next(head);                    // 顺序打印链表
  show_prev(head);                    // 逆序打印链表
  del_head(&head);                    // 删除链表头节点
  del_head(&head);                    // 删除链表头节点
  del_head(&head);                    // 删除链表头节点
  del_head(&head);                    // 删除链表头节点
  del_head(&head);                    // 删除链表头节点
  del_head(&head);                    // 删除链表头节点

  show_next(head);  // 顺序打印链表
  return 0;
}
