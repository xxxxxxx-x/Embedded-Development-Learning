/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0805/demo2/main.c
 * @brief        : 小练习：设计队列示例程序
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-05 11:25:31
 * @version      : 1.0
 * @note         :
 *示例代码展示如何初始化队列、执行入队和出队操作，并在程序结束时释放队列内存
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// 定义队列结构体
struct Queue {
  int max_length;   // 队列的最大长度
  int current_size; // 当前队列中的元素数量
  int *elements;    // 存储队列元素的数组
};

// 初始化队列
void init_queue(struct Queue *queue, int max_length) {
  queue->max_length = max_length;
  queue->current_size = 0;
  queue->elements = (int *)malloc(sizeof(int) * max_length);
  if (queue->elements == NULL) {
    printf("内存分配失败\n");
    exit(EXIT_FAILURE);
  }
}

// 入队操作
void enqueue(struct Queue *queue, int value) {
  if (queue->current_size >= queue->max_length) {
    printf("入队失败，队列已满！\n");
    return;
  }
  queue->elements[queue->current_size++] = value;
}

// 出队操作
void dequeue(struct Queue *queue) {
  if (queue->current_size == 0) {
    printf("出队失败，队列为空！\n");
    return;
  }

  int value = queue->elements[0];
  for (int i = 0; i < queue->current_size - 1; i++) {
    queue->elements[i] = queue->elements[i + 1];
  }
  queue->current_size--;
  printf("出队元素: %d\n", value);
}

// 释放队列内存
void free_queue(struct Queue *queue) {
  free(queue->elements);
  queue->elements = NULL;
  queue->current_size = 0;
  queue->max_length = 0;
}

int main(int argc, const char *argv[]) {
  struct Queue queue;

  // 初始化队列，最大长度为5
  init_queue(&queue, 5);

  // 入队操作
  enqueue(&queue, 1);
  enqueue(&queue, 2);
  enqueue(&queue, 3);
  enqueue(&queue, 4);
  enqueue(&queue, 5);
  enqueue(&queue, 6); // 超出队列容量，提示入队失败

  // 出队操作
  dequeue(&queue);
  dequeue(&queue);
  dequeue(&queue);
  dequeue(&queue);
  dequeue(&queue);
  dequeue(&queue); // 队列为空，提示出队失败

  // 释放队列内存
  free_queue(&queue);

  return 0;
}
