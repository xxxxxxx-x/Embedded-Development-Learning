/****************************************************************************************************************************************************************
 * @file         : /Code/Projects/DineSimple/src/Device/device.c
 * @brief        : 初始化 LCD 和触摸屏设备
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-15 15:44:43
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "../../include/Device/device.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/****************************************************************************************************************************************************************
 * @name: init_lcd
 * @brief: 初始化 LCD 设备，打开设备文件并将其映射到内存
 * @return: void* 映射到 LCD 设备的内存指针
 * @date: 2024-08-15 19:55:07
 * @version: 1.0
 * @note: 如果初始化失败，程序将调用 exit(EXIT_FAILURE) 终止执行
 *****************************************************************************************************************************************************************/
void* init_lcd(void) {
  // 打开 LCD 设备文件
  int fd = open(FB_PATH, O_RDWR);
  if (fd < 0) {
    perror("打开 LCD 设备失败");
    exit(EXIT_FAILURE);
  } else {
    printf("打开 LCD 设备成功\n");
  }

  // 映射 LCD 设备到内存
  void* lcd_memory =
      mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (lcd_memory == MAP_FAILED) {
    perror("映射 LCD 设备失败");
    close(fd);
    exit(EXIT_FAILURE);
  } else {
    printf("LCD 设备映射成功\n");
  }

  // 关闭文件描述符
  close(fd);
  return lcd_memory;
}

/****************************************************************************************************************************************************************
 * @name: init_touchscreen
 * @brief: 初始化触摸屏设备，打开触摸屏设备文件
 * @return: int 触摸屏设备文件的文件描述符
 * @date: 2024-08-15 19:55:14
 * @version: 1.0
 * @note: 如果初始化失败，程序将调用 exit(EXIT_FAILURE) 终止执行
 *****************************************************************************************************************************************************************/
int init_touchscreen(void) {
  // 打开触摸屏设备文件
  int device_fd = open(TOUCHSCREEN_PATH, O_RDWR);
  if (device_fd < 0) {
    perror("打开 触摸屏 设备失败");
    exit(EXIT_FAILURE);
  } else {
    printf("打开 触摸屏 设备成功\n");
  }
  return device_fd;
}
