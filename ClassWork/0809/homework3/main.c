/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/homework3/main.c
 * @brief        : 作业：实现一个程序，将需要显示的图像存储在双向链表中，
 *                 提供以下功能：
 *                 1. 输入 1 显示下一张图像
 *                 2. 输入 2 显示上一张图像
 *                 3. 输入 3 遍历链表循环显示所有图像
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-11 17:00:34
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include "dclist.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#if 1
#define FB "/dev/fb0"
#else
#define FB "/dev/ubuntu_lcd"
#endif

// 定义 BMP 文件头结构体
typedef struct {
  unsigned short bfType;      // 文件类型，通常为0x4D42（'BM'）
  unsigned int bfSize;        // 文件大小，以字节为单位
  unsigned short bfReserved1; // 保留字段，通常为0
  unsigned short bfReserved2; // 保留字段，通常为0
  unsigned int bfOffBits;     // 从文件头到图像数据的偏移量
} __attribute__((packed)) BITMAPFILEHEADER;

// 定义 BMP 位图信息头结构体
typedef struct {
  unsigned int biSize;  // 位图信息头的大小，通常为40字节
  unsigned int biWidth; // 图像宽度，以像素为单位
  unsigned int biHeight; // 图像高度，以像素为单位，正数表示倒置图像
  unsigned short biPlanes; // 总是设置为1
  unsigned short biBitCount; // 每个像素的位数，常见值有1、4、8、16、24和32
  unsigned int
      biCompression; // 压缩类型，常见值有BI_RGB（无压缩）、BI_RLE8、BI_RLE4、BI_BITFIELDS等
  unsigned int biSizeImage;     // 图像数据的大小，以字节为单位
  unsigned int biXPelsPerMeter; // 水平分辨率，单位为像素/米
  unsigned int biYPelsPerMeter; // 垂直分辨率，单位为像素/米
  unsigned int biClrUsed; // 实际使用的颜色数，如果为0则默认2的biBitCount次方
  unsigned int biClrImportant; // 重要颜色数，如果为0则所有颜色都重要
} __attribute__((packed)) BITMAPINFOHEADER;

// 映射 LCD 设备
void *mmap_lcd() {
  // 打开 LCD 设备
  int fd = open(FB, O_RDWR);
  if (fd < 0) {
    perror("打开LCD失败");
    return NULL;
  }

  // 映射 LCD 设备
  void *p =
      mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p == MAP_FAILED) {
    perror("映射失败");
    close(fd);
    return NULL;
  } else {
    printf("映射LCD成功\n");
  }

  close(fd);
  return p;
}

// 从 BMP 文件中读取图像数据并显示到 LCD 屏幕
int display_bmp_to_lcd(const char *filepath) {
  // 打开 BMP 图片文件
  int fd = open(filepath, O_RDONLY);
  if (fd < 0) {
    perror("打开文件失败");
    return EXIT_FAILURE;
  }

  // 读取 BMP 文件头
  BITMAPFILEHEADER fileHeader;
  if (read(fd, &fileHeader, sizeof(BITMAPFILEHEADER)) !=
      sizeof(BITMAPFILEHEADER)) {
    perror("读取文件头失败");
    close(fd);
    return EXIT_FAILURE;
  }

  // 读取 BMP 位图信息头
  BITMAPINFOHEADER infoHeader;
  if (read(fd, &infoHeader, sizeof(BITMAPINFOHEADER)) !=
      sizeof(BITMAPINFOHEADER)) {
    perror("读取位图信息头失败");
    close(fd);
    return EXIT_FAILURE;
  }

  // 输出 BMP 文件头信息
  printf("BMP 文件大小: %u 字节\n", fileHeader.bfSize);
  printf("图像宽度: %u 像素\n", infoHeader.biWidth);
  printf("图像高度: %u 像素\n", infoHeader.biHeight);
  printf("色深: %u 位\n", infoHeader.biBitCount);

  // 分配内存来存储图像的像素数据
  size_t imageSize =
      infoHeader.biWidth * infoHeader.biHeight * infoHeader.biBitCount / 8;
  char *rgb = malloc(imageSize);
  if (!rgb) {
    perror("内存分配失败");
    close(fd);
    return EXIT_FAILURE;
  }

  // 读取图像像素数据
  if (read(fd, rgb, imageSize) != (ssize_t)imageSize) {
    perror("读取像素数据失败");
    free(rgb);
    close(fd);
    return EXIT_FAILURE;
  }

  // 内存映射 LCD 设备
  int(*lcd)[800] = mmap_lcd();
  if (!lcd) {
    free(rgb);
    return EXIT_FAILURE;
  }

  // 将 RGB 数据转换为 LCD 可用的颜色格式
  for (int y = 0; y < infoHeader.biHeight; y++) {
    for (int x = 0; x < infoHeader.biWidth; x++) {
      char b = rgb[(y * infoHeader.biWidth + x) * 3 + 0];
      char g = rgb[(y * infoHeader.biWidth + x) * 3 + 1];
      char r = rgb[(y * infoHeader.biWidth + x) * 3 + 2];
      lcd[infoHeader.biHeight - y - 1][x] =
          b | (g << 8) | (r << 16) | (0 << 24);
    }
  }

  // 释放资源
  free(rgb);
  close(fd);

  return EXIT_SUCCESS;
}

void display_next_image(struct list_head *head, struct list_head **current) {
  if (*current == head->prev) {
    printf("已是最后一张图像\n");
    return;
  }

  *current = (*current)->next;
  ImageList *node = LIST_ENTRY(*current, ImageList, list);
  display_bmp_to_lcd((char *)node->filepath);
}

void display_previous_image(struct list_head *head,
                            struct list_head **current) {
  if (*current == head->next) {
    printf("已是第一张图像\n");
    return;
  }

  *current = (*current)->prev;
  ImageList *node = LIST_ENTRY(*current, ImageList, list);
  display_bmp_to_lcd((char *)node->filepath);
}

void display_all_images(struct list_head *head) {
  struct list_head *pos;

  LIST_FOR_EACH(pos, head) {
    ImageList *node = LIST_ENTRY(pos, ImageList, list);
    display_bmp_to_lcd((char *)node->filepath);
    sleep(1); // 每张图像显示 1 秒
  }
}

int main(int argc, char *argv[]) {
  // 初始化链表头
  LIST_HEAD(image_list);

  // 添加图像路径到链表
  insert_node(&image_list, "1.bmp");
  insert_node(&image_list, "2.bmp");
  insert_node(&image_list, "1.bmp");
  insert_node(&image_list, "2.bmp");
  insert_node(&image_list, "1.bmp");
  insert_node(&image_list, "2.bmp");
  insert_node(&image_list, "1.bmp");
  insert_node(&image_list, "2.bmp");
  // insert_node(&image_list, "3.bmp");

  // 当前显示的图像节点
  struct list_head *current = &image_list;

  while (1) {
    int choice;
    printf("输入选项 (1: 显示下一张, 2: 显示上一张, 3: 循环显示所有图像, 0: "
           "退出): ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      display_next_image(&image_list, &current);
      break;
    case 2:
      display_previous_image(&image_list, &current);
      break;
    case 3:
      display_all_images(&image_list);
      break;
    case 0:
      return 0;
    default:
      printf("无效选项\n");
      break;
    }
  }

  return 0;
}
