/****************************************************************************************************************************************************************
 * @file         : /DineSimple/src/Tasks/tasks.c
 * @brief        : 包含线程相关的任务函数实现，如时间获取线程和触摸屏任务线程
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-15 20:28:06
 * @version      : 1.0
 * @note         : 该文件实现了创建线程、获取时间和处理触摸屏事件的函数
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "../../include/Tasks/tasks.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化线程数组和计数器
Thread thread_array[MAX_THREADS];
int thread_count = 0;

// 条件变量和互斥锁的定义和初始化
pthread_cond_t screen_cond = PTHREAD_COND_INITIALIZER;  // 条件变量初始化
pthread_mutex_t screen_mutex = PTHREAD_MUTEX_INITIALIZER;  // 互斥锁初始化


/****************************************************************************************************************************************************************
 * @name: create_thread
 * @brief: 创建并启动一个线程
 * @param {void (*thread_func)(void *)} thread_func -
 *线程函数的指针，该函数接受一个 void* 类型的参数，并没有返回值
 * @param {void*} arg - 传递给线程函数的参数
 * @param {const char*} name - 线程名称
 * @param {int} group_id - 线程组标识符
 * @return {pthread_t} - 返回创建的线程标识符
 * @date: 2024-08-16 14:26:41
 * @version: 1.1
 * @note:
 * 该函数使用指定的线程函数、参数、名称和组ID创建一个新的线程，并启动该线程。如果线程创建失败，会打印错误信息并终止程序。
 *****************************************************************************************************************************************************************/
pthread_t create_thread(void (*thread_func)(void*),
                        void* arg,
                        const char* name,
                        int group_id) {
  pthread_t thread;

  // 创建线程并启动执行
  if (pthread_create(&thread, NULL, (void* (*)(void*))thread_func, arg) != 0) {
    perror("创建线程失败");
    exit(EXIT_FAILURE);
  }

  // 保存线程信息
  if (thread_count < MAX_THREADS) {
    thread_array[thread_count].thread_id = thread;
    thread_array[thread_count].thread_func = thread_func;
    thread_array[thread_count].arg = arg;
    strncpy(thread_array[thread_count].name, name,
            sizeof(thread_array[thread_count].name) - 1);
    thread_array[thread_count]
        .name[sizeof(thread_array[thread_count].name) - 1] =
        '\0';  // 确保名称以 null 结尾
    thread_array[thread_count].group_id = group_id;

    thread_count++;
  } else {
    fprintf(stderr, "线程数组已满，无法添加新线程。\n");
  }

  printf("成功添加函数 %p 到线程中\n", thread_func);
  return thread;
}

/****************************************************************************************************************************************************************
 * @name: destroy_thread
 * @brief: 销毁一个已经创建完成且在运行中的线程
 * @param {pthread_t} thread - 要销毁的线程标识符
 * @return {void} - 无返回值
 * @date: 2024-08-17
 * @version: 1.0
 * @note:
 * 该函数使用线程标识符销毁一个已经创建完成且在运行中的线程。如果线程销毁失败，会打印错误信息并终止程序。
 *****************************************************************************************************************************************************************/
void destroy_thread(pthread_t thread) {
  int result = pthread_cancel(thread);
  if (result != 0) {
    fprintf(stderr, "错误: 无法取消线程。\n");
    exit(EXIT_FAILURE);
  }

  result = pthread_join(thread, NULL);
  if (result != 0) {
    fprintf(stderr, "错误: 无法加入线程。\n");
    exit(EXIT_FAILURE);
  }
}

/****************************************************************************************************************************************************************
 * @name: find_thread_by_name
 * @brief: 根据线程名称查找线程标识符
 * @param {const char*} name - 线程名称
 * @return {pthread_t} - 线程标识符，如果找不到返回 NULL
 * @date: 2024-08-17
 * @version: 1.0
 * @note:
 * 该函数遍历线程数组，查找具有指定名称的线程，并返回其标识符。如果没有找到对应的线程，返回
 *NULL。
 *****************************************************************************************************************************************************************/
pthread_t find_thread_by_name(const char* name) {
  for (int i = 0; i < thread_count; ++i) {
    if (strcmp(thread_array[i].name, name) == 0) {
      return thread_array[i].thread_id;
    }
  }
  return (pthread_t)NULL;
}

/****************************************************************************************************************************************************************
 * @name: destroy_threads_by_group
 * @brief: 销毁指定线程组的所有线程
 * @param {int} group_id - 线程组标识符
 * @return {void} - 无返回值
 * @date: 2024-08-17
 * @version: 1.0
 * @note:
 * 该函数遍历线程数组，销毁所有属于指定线程组的线程。
 *****************************************************************************************************************************************************************/
void destroy_threads_by_group(int group_id) {
  for (int i = 0; i < thread_count; ++i) {
    if (thread_array[i].group_id == group_id) {
      destroy_thread(thread_array[i].thread_id);
      // 删除线程后移动后续线程
      for (int j = i; j < thread_count - 1; ++j) {
        thread_array[j] = thread_array[j + 1];
      }
      thread_count--;  // 减少线程计数器
      i--;             // 调整索引以处理移除后的线程
    }
  }
}
