/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0808/homework1/main.c
 * @brief        : 作业：实现开发板LCD屏幕的颜色循环播放。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-08 18:38:04
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// 生成 min 到 max 范围内的随机数
int generate_random_color(int min, int max) {
  // 设置随机数种子
  srand(time(NULL));
  return (rand() % (max - min + 1)) + min;
}

int main(int argc, const char *argv[]) {

  // 无限循环，用于不断更新LCD屏幕颜色
  while (1) {
    // 打开LCD设备文件
    int lcd_fd = open("/dev/fb0", O_RDWR);

    // 检查文件是否成功打开
    if (lcd_fd > 0) {
      // printf("打开LCD驱动成功\n");
    } else {
      // perror("打开LCD驱动失败");
      return 1; // 打开失败时退出程序
    }

    // 创建一个数组用于存储屏幕的像素颜色值
    int lcd_color[480][800] = {0};

    // 遍历每个像素点，设置随机颜色
    for (int y = 0; y < 480; y++) {

      for (int x = 0; x < 800; x++) {
        lcd_color[y][x] = generate_random_color(0, 0xFFFFFF);
      }
    }

    // 将颜色数据写入LCD设备文件
    ssize_t bytes_written = write(lcd_fd, lcd_color, sizeof(lcd_color));
    if (bytes_written == -1) {
      perror("写入LCD驱动失败");
      close(lcd_fd); // 关闭文件描述符
      return 1;      // 写入失败时退出程序
    }

    // 关闭LCD设备文件
    close(lcd_fd);

    // 延时一段时间，避免过于频繁地更新屏幕颜色
    sleep(1);
  }

  return 0;
}
