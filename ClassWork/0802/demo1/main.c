/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0802/demo1/main.c
 * @brief        : 小练习：用双向循环链表存储若干自然数，并将其奇偶重排输出
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-02 10:00:26
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "dclist.h"
#include <stdio.h>
#include <stdlib.h>

// 主函数入口
int main(int argc, const char *argv[]) {
  // 初始化链表头指针
  DCListNode *head = NULL;
  // 向链表头部插入若干自然数
  for (int i = 0; i <= 10; i++) {
    insert_dcnode_tail(&head, i);
  }

  // 打印原链表
  printf("原链表->");
  display_dlist(head);

  // 初始化奇数链表和偶数链表的头尾指针
  DCListNode *odd_list_head = NULL;
  DCListNode *even_list_head = NULL;
  DCListNode *odd_list_tail = NULL;
  DCListNode *even_list_tail = NULL;
  DCListNode *temp = head;

  // 遍历原链表并将奇数和偶数分开
  do {
    if (temp->data % 2 == 0) {
      // 偶数节点处理
      if (!even_list_head) {
        // 如果偶数链表为空，创建新节点并初始化头尾指针
        even_list_head = create_dcnode(temp->data);
        even_list_tail = even_list_head;
      } else {
        // 插入偶数链表尾部
        insert_dcnode_tail(&even_list_tail, temp->data);
      }
    } else {
      // 奇数节点处理
      if (!odd_list_head) {
        // 如果奇数链表为空，创建新节点并初始化头尾指针
        odd_list_head = create_dcnode(temp->data);
        odd_list_tail = odd_list_head;
      } else {
        // 插入奇数链表尾部
        insert_dcnode_tail(&odd_list_tail, temp->data);
      }
    }
    temp = temp->next;
  } while (temp != head);

  even_list_tail = even_list_tail->prev;
  odd_list_tail = odd_list_tail->prev;

  // 合并奇数链表和偶数链表
  if (odd_list_tail && even_list_head) {
    // 如果奇数链表和偶数链表均不为空
    odd_list_tail->next = even_list_head;
    even_list_head->prev = odd_list_tail;
    even_list_tail->next = odd_list_head;
    odd_list_head->prev = even_list_tail;
    head = odd_list_head;
  } else if (odd_list_tail) {
    // 如果只有奇数链表不为空
    odd_list_tail->next = odd_list_head;
    odd_list_head->prev = odd_list_tail;
    head = odd_list_head;
  } else {
    // 如果只有偶数链表不为空
    even_list_tail->next = even_list_head;
    even_list_head->prev = even_list_tail;
    head = even_list_head;
  }

  // 打印重排后的链表
  printf("重排后的链表->");
  display_dlist(head);

  // 释放链表内存
  // free_dclist(head);

  return 0;
}
