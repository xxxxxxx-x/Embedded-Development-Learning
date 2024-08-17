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
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <pthread.h>  // 引入 POSIX 线程库头文件
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/UI/ui.h"

/****************************************************************************************************************************************************************
 * @name: ts_task
 * @brief: 读取触摸屏输入事件并处理触摸坐标
 * @param {void*} arg - 参数是一个指向 UI 结构体的指针
 * @return {void} - 无返回值
 * @date: 2024-08-16 14:47:30
 * @version: 1.2
 * @note: 该函数会持续运行，实时处理触摸事件和更新坐标
 *****************************************************************************************************************************************************************/
void ts_task(void* arg) {
  // 将参数转换为 UI* 类型的指针
  UI* ui = (UI*)arg;

  // 无限循环读取触摸屏输入事件
  while (1) {
    struct input_event input_event;

    // 从设备文件中读取输入事件
    ssize_t bytes_read = read(ui->input, &input_event, sizeof(input_event));
    if (bytes_read < 0) {
      perror("读取输入触摸事件失败");
      close(ui->input);
      return;
    }

    // 临时存储触摸坐标
    static int x = -1, y = -1;

    // 处理 X 轴触摸事件
    if (input_event.type == EV_ABS && input_event.code == ABS_X) {
      x = 800 * input_event.value / 1024;  // 将原始 X 轴值转换为屏幕坐标
    }

    // 处理 Y 轴触摸事件
    if (input_event.type == EV_ABS && input_event.code == ABS_Y) {
      y = 480 * input_event.value / 600;  // 将原始 Y 轴值转换为屏幕坐标
    }

    // 处理按键事件
    if (input_event.type == EV_KEY && input_event.code == BTN_TOUCH) {
      if (input_event.value == 1) {  // 触摸按下事件
        ui->ts_x = x;                // 更新 X 坐标
        ui->ts_y = y;                // 更新 Y 坐标
        // printf("x:%d y:%d\n", ui->ts_x, ui->ts_y);
      }
      // 可以选择在触摸松开事件中添加其他逻辑
    }
  }

  return;
}

/****************************************************************************************************************************************************************
 * @name: ts_get_time
 * @brief: 获取当前时间并更新 UI 显示
 * @param {void*} arg - 参数是一个指向 UI 结构体的指针
 * @return {void} - 无返回值
 * @date: 2024-08-16 14:47:30
 * @version: 1.0
 * @note: 该函数会持续运行，定时更新 UI 显示的时间
 *****************************************************************************************************************************************************************/
void ts_get_time(void* arg) {
  UI* ui = (UI*)arg;

  // 无限循环更新时间
  while (1) {
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);
    strftime(ui->time_str, sizeof(ui->time_str), "%Y-%m-%d %H:%M:%S",
             local_time);

    // 更新 UI 显示时间
    // 这里可以添加代码来刷新 UI 上的时间显示

    sleep(1);  // 每秒更新一次
  }
}

/****************************************************************************************************************************************************************
 * @name: create_thread
 * @brief: 创建并启动一个线程
 * @param {void (*thread_func)(void *)} thread_func -
 *线程函数的指针，该函数接受一个 void* 类型的参数，并没有返回值
 * @param {void*} arg - 传递给线程函数的参数
 * @return {void} - 无返回值
 * @date: 2024-08-16 14:26:41
 * @version: 1.0
 * @note:
 *该函数使用指定的线程函数和参数创建一个新的线程，并启动该线程。如果线程创建失败，会打印错误信息并终止程序。
 *****************************************************************************************************************************************************************/
void create_thread(void (*thread_func)(void*), void* arg) {
  pthread_t thread;

  // 创建线程并启动执行
  if (pthread_create(&thread, NULL, (void* (*)(void*))thread_func, arg) != 0) {
    perror("创建线程失败");
    exit(EXIT_FAILURE);
  }
  printf("线程函数 %p 添加成功\n", thread_func);
}
