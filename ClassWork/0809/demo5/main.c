/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/demo5/main.c
 * @brief        : 小练习：打开帧缓冲设备并将其内存映射到进程地址空间
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-09 15:24:34
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>    // 文件控制定义
#include <stdio.h>    // 标准输入输出定义
#include <stdlib.h>   // 标准库定义
#include <sys/mman.h> // 内存映射定义
#include <unistd.h>   // Unix 标准函数定义

#define FB0_PATH "/dev/fb0" // 帧缓冲设备路径
#define SCREEN_WIDTH 800    // 屏幕宽度
#define SCREEN_HEIGHT 480   // 屏幕高度
#define PIXEL_SIZE 4        // 每个像素的字节数（假设为 RGBA）

int main(int argc, const char *argv[]) {

  // 打开帧缓冲设备文件
  int fb0 = open(FB0_PATH, O_RDWR);
  if (fb0 < 0) {
    perror("无法打开帧缓冲设备");
    exit(EXIT_FAILURE);
  }

  // 计算屏幕总大小（字节）
  size_t screen_size = SCREEN_WIDTH * SCREEN_HEIGHT * PIXEL_SIZE;

  // 将帧缓冲设备映射到进程地址空间
  void *p = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb0, 0);
  if (p == MAP_FAILED) {
    perror("内存映射失败");
    close(fb0); // 关闭文件描述符
    exit(EXIT_FAILURE);
  }

  // 显示映射成功的信息
  printf("内存映射成功\n");

  // 在完成操作后，解除映射
  if (munmap(p, screen_size) < 0) {
    perror("解除内存映射失败");
  }

  // 关闭帧缓冲设备
  close(fb0);

  return 0;
}
