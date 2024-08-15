/****************************************************************************************************************************************************************
 * @file         : /Code/Projects/DineSimple/src/image_display.c
 * @brief        : 从图像文件中读取数据并缩放显示到 LCD 屏幕
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-09 16:30:00
 * @version      : 1.1
 * @note         : 需要提供一个 BMP 文件路径作为命令行参数
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "../include/image_display.h"

/**
 * @brief 从图像文件中读取数据并缩放显示到 LCD 屏幕
 *
 * @param lcd_mmap  映射到 LCD 的内存指针
 * @param imagePath 图像文件路径
 * @param x         显示图像的起始 X 坐标
 * @param y         显示图像的起始 Y 坐标
 * @param scale     图像缩放比例
 */
void display_image(int* lcd_mmap,
                   const char* imagePath,
                   int x,
                   int y,
                   double scale) {
  // 打开图像文件
  int fd = open(imagePath, O_RDONLY);
  if (fd < 0) {
    char errorMsg[256];
    snprintf(errorMsg, sizeof(errorMsg), "打开图片失败: %s", imagePath);
    perror(errorMsg);
    return;
  }

  // 读取图像文件头
  BITMAPFILEHEADER fileHeader;
  if (read(fd, &fileHeader, sizeof(BITMAPFILEHEADER)) !=
      sizeof(BITMAPFILEHEADER)) {
    perror("读取文件头失败");
    close(fd);
    return;
  }

  // 读取图像位图信息头
  BITMAPINFOHEADER infoHeader;
  if (read(fd, &infoHeader, sizeof(BITMAPINFOHEADER)) !=
      sizeof(BITMAPINFOHEADER)) {
    perror("读取位图信息头失败");
    close(fd);
    return;
  }

  // 输出图像文件头信息
  printf("图像文件大小: %u 字节\n", fileHeader.bfSize);
  printf("图像宽度: %u 像素\n", infoHeader.biWidth);
  printf("图像高度: %u 像素\n", infoHeader.biHeight);
  printf("色深: %u 位\n", infoHeader.biBitCount);

  // 计算图像数据大小并分配内存
  size_t imageSize =
      infoHeader.biWidth * infoHeader.biHeight * infoHeader.biBitCount / 8;
  char* rgb = malloc(imageSize);
  if (!rgb) {
    perror("内存分配失败");
    close(fd);
    return;
  }

  // 读取图像像素数据
  if (lseek(fd, fileHeader.bfOffBits, SEEK_SET) == -1 ||
      read(fd, rgb, imageSize) != (ssize_t)imageSize) {
    perror("读取像素数据失败");
    free(rgb);
    close(fd);
    return;
  }

  // 内存映射 LCD 设备
  int(*lcd)[800] = (int(*)[800])lcd_mmap;

  // 计算缩放后的宽度和高度
  int scaledWidth = (int)(infoHeader.biWidth * scale);
  int scaledHeight = (int)(infoHeader.biHeight * scale);

  // 将 RGB 数据转换为 LCD 可用的颜色格式，并将其显示到 LCD 屏幕
  for (int lcdY = y, bmpY = 0;
       lcdY < y + scaledHeight && bmpY < (int)infoHeader.biHeight;
       lcdY++, bmpY = (int)(bmpY + 1 / scale)) {
    for (int lcdX = x, bmpX = 0;
         lcdX < x + scaledWidth && bmpX < (int)infoHeader.biWidth;
         lcdX++, bmpX = (int)(bmpX + 1 / scale)) {
      // 计算 RGB 颜色值
      char b = rgb[(bmpY * infoHeader.biWidth + bmpX) * 3 + 0];
      char g = rgb[(bmpY * infoHeader.biWidth + bmpX) * 3 + 1];
      char r = rgb[(bmpY * infoHeader.biWidth + bmpX) * 3 + 2];

      // 设置 LCD 像素值（将 RGB 转换为 ARGB 格式）
      lcd[infoHeader.biHeight - lcdY - 1][lcdX] =
          b | (g << 8) | (r << 16) | (0 << 24);
    }
  }

  // 释放资源
  free(rgb);
  close(fd);
}
