/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0730/homework1/main.c
 * @brief        : 测试单链表的各个函数接口
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-30 16:21:20
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "list.h"
#include <stdio.h>

int main() {
  ListNode *head = NULL;

  // 测试头部插入
  insert_head(&head, 10);
  insert_head(&head, 20);
  display_list(head); // 输出应为：20 -> 10 -> NULL

  // 测试尾部插入
  insert_tail(&head, 30);
  display_list(head); // 输出应为：20 -> 10 -> 30 -> NULL

  // 测试指定位置插入
  insert_at(&head, 25, 2);
  display_list(head); // 输出应为：20 -> 10 -> 25 -> 30 -> NULL

  // 测试删除头部节点
  delete_head(&head);
  display_list(head); // 输出应为：10 -> 25 -> 30 -> NULL

  // 测试删除尾部节点
  delete_tail(&head);
  display_list(head); // 输出应为：10 -> 25 -> NULL

  // 测试指定位置删除
  delete_at(&head, 1);
  display_list(head); // 输出应为：10 -> NULL

  // 测试更新节点值
  insert_tail(&head, 40);
  update_value(head, 1, 50);
  display_list(head); // 输出应为：10 -> 50 -> NULL

  // 测试查找节点
  ListNode *found = search_value(head, 10);
  if (found) {
    printf("找到值为10的节点\n");
  } else {
    printf("未找到值为10的节点\n");
  }

  return 0;
}
