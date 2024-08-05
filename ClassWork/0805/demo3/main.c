/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0805/demo3/main.c
 * @brief        : 小练习：设计链表队列
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-05 14:36:32
 * @version      : 1.0
 * @note         : 实现一个基于链表的队列，支持入队、出队及释放队列内存
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// 链表节点结构体定义
struct Node {
  int value;         // 节点存储的值
  struct Node *next; // 指向下一个节点的指针
};

// 队列结构体定义
struct Queue {
  int max_length;    // 队列的最大长度
  int current_size;  // 当前队列中的元素数量
  struct Node *head; // 队列的头指针（实际是链表的头节点）
  struct Node *tail; // 队列的尾指针
};

/****************************************************************************************************************************************************************
 * @name:        init_queue
 * @brief:       初始化队列
 * @param {Queue} *queue: 指向队列结构体的指针
 * @param {int} max_length: 队列的最大长度
 * @return:       无
 * @date:        2024-08-05 15:02:52
 * @version:     1.0
 * @note:        初始化队列的头和尾指针，设置最大长度和当前大小
 *****************************************************************************************************************************************************************/
void init_queue(struct Queue *queue, int max_length) {
  queue->max_length = max_length;
  queue->current_size = 0;
  queue->head = NULL; // 初始化头指针为空
  queue->tail = NULL; // 初始化尾指针为空
}

/****************************************************************************************************************************************************************
 * @name:        enqueue
 * @brief:       向队列中添加一个元素
 * @param {Queue} *queue: 指向队列结构体的指针
 * @param {int} value: 要添加的元素值
 * @return:       无
 * @date:        2024-08-05 15:03:14
 * @version:     1.0
 * @note:        添加元素到队列尾部，如果队列已满则提示失败
 *****************************************************************************************************************************************************************/
void enqueue(struct Queue *queue, int value) {
  if (queue->current_size >= queue->max_length) {
    printf("入队失败，队列已满！\n");
    return;
  }

  // 创建新节点并分配内存
  struct Node *new_node = malloc(sizeof(struct Node));
  if (new_node == NULL) {
    perror("内存分配失败");
    exit(EXIT_FAILURE);
  }

  new_node->value = value; // 设置节点的值
  new_node->next = NULL;   // 新节点的下一个指针初始化为空

  // 如果队列为空，新节点既是头节点也是尾节点
  if (queue->tail == NULL) {
    queue->head = new_node;
    queue->tail = new_node;
  } else {
    // 否则，将新节点添加到队列尾部，并更新尾指针
    queue->tail->next = new_node;
    queue->tail = new_node;
  }

  queue->current_size++; // 更新当前队列大小
}

/****************************************************************************************************************************************************************
 * @name:        dequeue
 * @brief:       从队列中删除并返回头部元素
 * @param {Queue} *queue: 指向队列结构体的指针
 * @return:       无
 * @date:        2024-08-05 15:03:21
 * @version:     1.0
 * @note:        删除队列头部的节点，如果队列为空则提示失败
 *****************************************************************************************************************************************************************/
void dequeue(struct Queue *queue) {
  if (queue->current_size == 0) {
    printf("出队失败，队列为空！\n");
    return;
  }

  // 取出头部节点并更新头指针
  struct Node *to_del = queue->head;
  printf("出队元素: %d\n", to_del->value);

  queue->head = to_del->next;

  // 如果队列变为空，将尾指针也设置为空
  if (queue->head == NULL) {
    queue->tail = NULL;
  }

  free(to_del);          // 释放删除的节点内存
  queue->current_size--; // 更新当前队列大小
}

/****************************************************************************************************************************************************************
 * @name:        free_queue
 * @brief:       释放队列中所有节点的内存
 * @param {Queue} *queue: 指向队列结构体的指针
 * @return:       无
 * @date:        2024-08-05 15:03:29
 * @version:     1.0
 * @note:        释放所有节点的内存并重置队列属性
 *****************************************************************************************************************************************************************/
void free_queue(struct Queue *queue) {
  struct Node *temp = queue->head;
  while (temp) {
    struct Node *to_free = temp;
    temp = temp->next;
    free(to_free); // 逐个释放节点内存
  }

  queue->head = NULL;      // 重置头指针
  queue->tail = NULL;      // 重置尾指针
  queue->current_size = 0; // 重置当前大小
  queue->max_length = 0;   // 重置最大长度
}

/****************************************************************************************************************************************************************
 * @name:        main
 * @brief:       测试链表队列的基本操作
 * @param {int} argc: 命令行参数的数量
 * @param {const char*} argv: 命令行参数数组
 * @return:       0: 程序成功执行
 * @date:        2024-08-05 15:03:36
 * @version:     1.0
 * @note:        初始化队列，进行入队和出队操作，最后释放队列内存
 *****************************************************************************************************************************************************************/
int main(int argc, const char *argv[]) {
  struct Queue queue;

  // 初始化队列，最大长度为 5
  init_queue(&queue, 5);

  // 向队列中添加元素
  enqueue(&queue, 1);
  enqueue(&queue, 2);
  enqueue(&queue, 3);
  enqueue(&queue, 4);
  enqueue(&queue, 5);

  // 尝试添加更多元素，应该提示队列已满
  enqueue(&queue, 6);

  // 执行出队操作
  dequeue(&queue);
  dequeue(&queue);
  dequeue(&queue);
  dequeue(&queue);
  dequeue(&queue);

  // 尝试从空队列中出队，应该提示队列为空
  dequeue(&queue);

  // 释放队列资源
  free_queue(&queue);

  return 0; // 程序正常结束
}
