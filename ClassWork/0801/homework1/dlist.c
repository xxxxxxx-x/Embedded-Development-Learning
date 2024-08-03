/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0801/homework1/dlist.c
 * @brief        : 双链表的实现文件，包含双链表的操作函数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-01 14:45:00
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "dlist.h"
#include <stdio.h>

/****************************************************************************************************************************************************************
 * @name: create_dnode
 * @brief: 创建一个新的双链表节点
 * @param {int} value 节点数据
 * @return {DListNode*} 新创建的节点指针
 * @date: 2024-08-01 16:14:59
 * @version: 1.0
 * @note: 如果内存分配失败，将打印错误信息并退出程序
 *****************************************************************************************************************************************************************/
DListNode *create_dnode(int value) {
    // 分配内存给新节点
    DListNode *new_node = (DListNode *)malloc(sizeof(DListNode));
    if (new_node == NULL) {
        // 内存分配失败，打印错误信息并退出
        perror("创建节点失败");
        exit(EXIT_FAILURE);
    }
    // 初始化新节点的值和指针
    new_node->data = value;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

/****************************************************************************************************************************************************************
 * @name: insert_head
 * @brief: 在双链表的头部插入一个新节点
 * @param {DListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 16:15:12
 * @version: 1.0
 * @note: 如果链表为空，新节点将成为头节点；否则，将新节点插入到链表头部
 *****************************************************************************************************************************************************************/
bool insert_dnode_head(DListNode **head, int value) {
    if (head == NULL) return false;

    // 创建新节点
    DListNode *new_node = create_dnode(value);
    if (*head == NULL) {
        // 如果链表为空，将新节点设为头节点
        *head = new_node;
    } else {
        // 将新节点插入到链表头部
        new_node->next = *head;
        (*head)->prev = new_node;
        *head = new_node;
    }
    return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_tail
 * @brief: 在双链表的尾部插入一个新节点
 * @param {DListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @return {bool} 插入成功返回 true，否则返回 false
 * @date: 2024-08-01 16:15:19
 * @version: 1.0
 * @note: 如果链表为空，新节点将成为头节点；否则，遍历链表并将新节点插入到链表尾部
 *****************************************************************************************************************************************************************/
bool insert_dnode_tail(DListNode **head, int value) {
    if (head == NULL) return false;

    // 创建新节点
    DListNode *new_node = create_dnode(value);
    if (*head == NULL) {
        // 如果链表为空，将新节点设为头节点
        *head = new_node;
    } else {
        // 遍历到链表尾部
        DListNode *temp = *head;
        while (temp->next) {
            temp = temp->next;
        }
        // 将新节点插入到链表尾部
        temp->next = new_node;
        new_node->prev = temp;
    }
    return true;
}

/****************************************************************************************************************************************************************
 * @name: insert_at
 * @brief: 在双链表的指定位置插入一个新节点
 * @param {DListNode**} head 链表头指针的指针
 * @param {int} value 新节点的数据
 * @param {int} position 插入位置
 * @return {void}
 * @date: 2024-08-01 16:15:26
 * @version: 1.0
 * @note: 如果位置为 0，将调用 `insert_head` 函数；否则，遍历链表并将新节点插入到指定位置
 *****************************************************************************************************************************************************************/
void insert_dnode_at(DListNode **head, int value, int position) {
    if (head == NULL || position < 0) return;

    if (position == 0) {
        // 在链表头部插入新节点
        insert_dnode_head(head, value);
        return;
    }

    // 创建新节点
    DListNode *new_node = create_dnode(value);
    DListNode *temp = *head;
    // 遍历链表找到插入位置的前一个节点
    for (int i = 0; i < position - 1 && temp != NULL; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        // 如果位置超出链表范围，打印错误信息并释放内存
        printf("插入失败：位置超出链表范围\n");
        free(new_node);
        return;
    }

    // 插入新节点
    new_node->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = new_node;
    }
    temp->next = new_node;
    new_node->prev = temp;
}

/****************************************************************************************************************************************************************
 * @name: delete_head
 * @brief: 删除双链表的头节点
 * @param {DListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 16:15:34
 * @version: 1.0
 * @note: 如果链表为空或头节点为空，什么都不做；否则，删除头节点并更新头指针
 *****************************************************************************************************************************************************************/
void delete_dnode_head(DListNode **head) {
    if (head == NULL || *head == NULL) return;

    // 保存头节点
    DListNode *temp = *head;
    // 更新头指针
    *head = (*head)->next;
    if (*head != NULL) {
        // 更新新的头节点的前驱指针
        (*head)->prev = NULL;
    }
    // 释放删除的节点
    free(temp);
}

/****************************************************************************************************************************************************************
 * @name: delete_tail
 * @brief: 删除双链表的尾节点
 * @param {DListNode**} head 链表头指针的指针
 * @return {void}
 * @date: 2024-08-01 16:15:40
 * @version: 1.0
 * @note: 如果链表为空，什么都不做；如果链表只有一个节点，删除后将头指针设为 NULL；否则，遍历链表找到尾节点并删除
 *****************************************************************************************************************************************************************/
void delete_dnode_tail(DListNode **head) {
    if (head == NULL || *head == NULL) return;

    DListNode *temp = *head;
    if (temp->next == NULL) {
        // 如果链表只有一个节点，删除后将头指针设为 NULL
        *head = NULL;
        free(temp);
        return;
    }

    // 遍历到链表尾部
    while (temp->next != NULL) {
        temp = temp->next;
    }

    // 删除尾节点
    temp->prev->next = NULL;
    free(temp);
}

/****************************************************************************************************************************************************************
 * @name: delete_at
 * @brief: 删除双链表指定位置的节点
 * @param {DListNode**} head 链表头指针的指针
 * @param {int} position 删除的位置
 * @return {void}
 * @date: 2024-08-01 16:15:48
 * @version: 1.0
 * @note: 如果位置为 0，将调用 `delete_head` 函数；否则，遍历链表找到指定位置的节点并删除
 *****************************************************************************************************************************************************************/
void delete_dnode_at(DListNode **head, int position) {
    if (head == NULL || *head == NULL || position < 0) return;

    DListNode *temp = *head;
    if (position == 0) {
        // 删除头节点
        delete_dnode_head(head);
        return;
    }

    // 遍历链表找到指定位置的节点
    for (int i = 0; i < position && temp != NULL; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        // 如果位置超出链表范围，打印错误信息
        printf("删除失败：位置超出链表范围\n");
        return;
    }

    // 删除节点
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    }
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }
    // 释放删除的节点
    free(temp);
}

/****************************************************************************************************************************************************************
 * @name: update_value
 * @brief: 更新双链表指定位置节点的数据
 * @param {DListNode*} head 链表头指针
 * @param {int} position 更新位置
 * @param {int} new_value 新数据值
 * @return {void}
 * @date: 2024-08-01 16:15:56
 * @version: 1.0
 * @note: 遍历链表找到指定位置的节点并更新其数据值；如果位置超出链表范围，打印错误信息
 *****************************************************************************************************************************************************************/
void update_dnode_value(DListNode *head, int position, int new_value) {
    if (head == NULL || position < 0) return;

    // 遍历链表找到指定位置的节点
    DListNode *temp = head;
    for (int i = 0; i < position && temp != NULL; i++) {
        temp = temp->next;
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
 * @param {DListNode*} head 链表头指针
 * @param {int} value 要查找的值
 * @return {DListNode*} 查找到的节点指针；如果未找到，则返回 NULL
 * @date: 2024-08-01 16:16:03
 * @version: 1.0
 * @note: 遍历链表查找指定值的节点
 *****************************************************************************************************************************************************************/
DListNode *search_dnode_value(DListNode *head, int value) {
    while (head != NULL) {
        if (head->data == value) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

/****************************************************************************************************************************************************************
 * @name: display_list
 * @brief: 打印双链表中的所有节点数据
 * @param {DListNode*} head 链表头指针
 * @return {void}
 * @date: 2024-08-01 16:16:14
 * @version: 1.0
 * @note: 遍历链表并打印节点数据，最后打印换行符
 *****************************************************************************************************************************************************************/
void display_dlist(DListNode *head) {
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}
