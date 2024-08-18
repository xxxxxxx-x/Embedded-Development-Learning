/****************************************************************************************************************************************************************
 * @file         : /DineSimple/src/UI/ui_tasks.c
 * @brief        :
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-17 14:06:01
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <pthread.h>  // 引入 POSIX 线程库头文件
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/Server/server.h"
#include "../../include/Tasks/tasks.h"
#include "../../include/Tools/tools.h"

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
        handle_touch_event(x, y);
        x = -1;
        y = -1;
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
  ui->date_time = malloc(sizeof(struct tm));
  if (ui->date_time == NULL) {
    fprintf(stderr, "内存分配失败：");
    perror(NULL);
    exit(EXIT_FAILURE);
  }

  // 无限循环更新时间
  while (1) {
    time_t now = time(NULL);                  // 获取当前时间
    struct tm* local_time = localtime(&now);  // 将当前时间转换为本地时间

    if (local_time != NULL) {
      // 将获取到的时间信息复制到 UI 结构体中
      *ui->date_time = *local_time;
    } else {
      // 处理获取时间失败的情况
      fprintf(stderr, "获取本地时间失败：");
      perror(NULL);
      exit(EXIT_FAILURE);
    }

    sleep(1);  // 每秒更新一次
  }
}

/****************************************************************************************************************************************************************
 * @name: welcome_display_time_tasks
 * @brief: 在欢迎页面上显示时间
 * @param {void*} arg - 指向 UI 结构体的指针
 * @return {void} - 无返回值
 * @date: 2024-08-17
 * @version: 1.0
 * @note:
 * 该函数在欢迎页面上实时更新显示时间。每秒更新一次时间。
 *****************************************************************************************************************************************************************/
void welcome_display_time_tasks(void* arg) {
  // 将参数转换为 UI 结构体指针
  UI* ui = ((UI*)arg);

  // 确保传入的参数有效
  if (ui == NULL || ui->date_time == NULL) {
    fprintf(stderr, "错误: 无效的 UI 参数或未初始化的时间。\n");
    return;
  }

  char time_str[20];
  char date_week_str[80];  // 存储日期和星期的字符串

  while (1) {
    // 确保 ui->date_time 已经被正确更新
    struct tm local_time = *(ui->date_time);

    // 格式化时间字符串
    snprintf(time_str, sizeof(time_str), "%02d:%02d", local_time.tm_hour,
             local_time.tm_min);

    // 格式化日期和星期字符串
    snprintf(date_week_str, sizeof(date_week_str), "%04d年%02d月%02d日 %s",
             local_time.tm_year + 1900, local_time.tm_mon + 1,
             local_time.tm_mday,
             (local_time.tm_wday == 0)   ? "星期日"
             : (local_time.tm_wday == 1) ? "星期一"
             : (local_time.tm_wday == 2) ? "星期二"
             : (local_time.tm_wday == 3) ? "星期三"
             : (local_time.tm_wday == 4) ? "星期四"
             : (local_time.tm_wday == 5) ? "星期五"
                                         : "星期六");

    // 显示时间
    show_text(ui->lcd_mmap, ui->font, time_str, 28, 0, 0, 70, 28,
              getColor24(255, 255, 255), 70, 690, 50, hexToRGB(0x474D51));

    // 显示日期和星期
    show_text(ui->lcd_mmap, ui->font, date_week_str, 14, 0, 0, 150, 14,
              getColor24(255, 255, 255), 150, 610, 80, hexToRGB(0x695C4E));

    // 休眠 30 秒钟
    sleep(30);
  }
}