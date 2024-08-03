/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0801/demo1/main.c
 * @brief        : 小练习：实现链表的奇偶划分，奇数在左，偶数在右
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-01 14:26:48
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************************************************************************************************
 * @name: func
 * @brief: 实现链表的奇偶划分，将奇数节点放在链表左侧，偶数节点放在链表右侧
 * @param {ListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 14:26:48
 * @version: 1.0
 * @note: 修改链表，使奇数节点在前，偶数节点在后，释放原链表的内存。
 *****************************************************************************************************************************************************************/
void func(ListNode **head) {
  // 检查输入链表是否为空
  if (head == NULL || *head == NULL) {
    return; // 如果链表为空，则直接返回
  }

  // 初始化奇数和偶数链表的尾节点
  ListNode *odd_head = NULL;  // 奇数链表的头节点
  ListNode *odd_tail = NULL;  // 奇数链表的尾节点
  ListNode *even_head = NULL; // 偶数链表的头节点
  ListNode *even_tail = NULL; // 偶数链表的尾节点

  ListNode *current = *head; // 当前遍历节点

  // 遍历原链表
  while (current) {
    // 判断当前节点的数据是否为偶数
    if (current->data % 2 == 0) {
      // 如果偶数链表为空，则初始化偶数链表的头和尾节点
      if (even_tail == NULL) {
        even_head = even_tail = create_node(current->data);
      } else {
        // 否则将新节点添加到偶数链表的尾部
        even_tail->next = create_node(current->data);
        even_tail = even_tail->next;
      }
    } else {
      // 如果奇数链表为空，则初始化奇数链表的头和尾节点
      if (odd_tail == NULL) {
        odd_head = odd_tail = create_node(current->data);
      } else {
        // 否则将新节点添加到奇数链表的尾部
        odd_tail->next = create_node(current->data);
        odd_tail = odd_tail->next;
      }
    }
    current = current->next; // 移动到下一个节点
  }

  // 将奇数链表与偶数链表连接起来
  if (odd_tail != NULL) {
    odd_tail->next = even_head;
  } else {
    odd_head = even_head; // 如果奇数链表为空，则直接将偶数链表作为新链表
  }

  // 释放原链表的内存
  current = *head;
  while (current) {
    ListNode *to_delete = current;
    current = current->next;
    free(to_delete);
  }

  // 更新头指针
  *head = odd_head;
}

int main() {
  ListNode *head = NULL;

  // 创建一个链表
  insert_head(&head, 10);
  insert_tail(&head, 20);
  insert_tail(&head, 30);
  insert_at(&head, 15, 1); // 在位置1插入15
  insert_at(&head, 25, 3); // 在位置3插入25

  // 显示链表
  printf("链表内容：");
  display_list(head);
  func(&head);
  display_list(head);

  // 清理链表
  while (head != NULL) {
    delete_head(&head);
  }

  return 0;
}