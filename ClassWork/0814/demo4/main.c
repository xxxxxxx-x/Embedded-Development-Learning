/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0814/demo4/main.c
 * @brief        : 小练习:显示一张jpeg图片到开发板中
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-15 09:31:52
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
int read_JPEG_file(char* filename);

// 全局变量，所有函数都可以访问
int (*lcd)[800] = NULL;

// 映射LCD设备
void* mmap_lcd() {
  // 1.打开LCD 设备
  int fd = open("/dev/fb0", O_RDWR);
  if (fd < 0) {
    perror("打开LCD失败\n");
    return NULL;
  }

  // 2.映射LCD设备
  void* p =
      mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p == MAP_FAILED) {
    perror("映射失败\n");
    return NULL;
  } else {
    printf("映射LCD成功\n");
  }

  return p;
}

int main() {
  // 映射LCD 设备
  lcd = mmap_lcd();

  printf("开始解码\n");

  read_JPEG_file("1.jpg");

  printf("解码结束\n");
}