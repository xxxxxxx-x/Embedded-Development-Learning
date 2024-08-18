/****************************************************************************************************************************************************************
 * @file         : /DineSimple/src/Tools/tools.c
 * @brief        : 实现图像显示、文本显示和字体初始化功能的工具函数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-16 16:34:51
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "../../include/Tools/tools.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/****************************************************************************************************************************************************************
 * @name: show_bitmap
 * @brief: 从图像文件中读取数据并缩放显示到 LCD 屏幕
 * @param {int*} lcd_mmap - 映射到 LCD 的内存指针
 * @param {const char*} imagePath - 图像文件路径
 * @param {int} x - 显示图像的起始 X 坐标
 * @param {int} y - 显示图像的起始 Y 坐标
 * @param {double} scale - 图像缩放比例（应为4的倍数）
 * @return: void
 * @note: 该函数读取指定路径的图像文件，将其缩放后显示在 LCD 屏幕的指定位置。
 *****************************************************************************************************************************************************************/
void show_bitmap(int* lcd_mmap,
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
    exit(EXIT_FAILURE);
  }

  // 读取图像文件头
  BITMAPFILEHEADER fileHeader;
  if (read(fd, &fileHeader, sizeof(BITMAPFILEHEADER)) !=
      sizeof(BITMAPFILEHEADER)) {
    perror("读取文件头失败");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // 读取图像位图信息头
  BITMAPINFOHEADER infoHeader;
  if (read(fd, &infoHeader, sizeof(BITMAPINFOHEADER)) !=
      sizeof(BITMAPINFOHEADER)) {
    perror("读取位图信息头失败");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // 输出图像文件头信息
  // printf("图像文件大小: %u 字节\n", fileHeader.bfSize);
  // printf("图像宽度: %u 像素\n", infoHeader.biWidth);
  // printf("图像高度: %u 像素\n", infoHeader.biHeight);
  // printf("色深: %u 位\n", infoHeader.biBitCount);

  // 计算图像数据大小并分配内存
  size_t imageSize =
      infoHeader.biWidth * infoHeader.biHeight * (infoHeader.biBitCount / 8);
  char* bmpData = malloc(imageSize);
  if (!bmpData) {
    perror("内存分配失败");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // 读取图像像素数据
  if (lseek(fd, fileHeader.bfOffBits, SEEK_SET) == -1 ||
      read(fd, bmpData, imageSize) != (ssize_t)imageSize) {
    perror("读取像素数据失败");
    free(bmpData);
    close(fd);
    exit(EXIT_FAILURE);
  }

  // 内存映射 LCD 设备
  int(*lcd)[800] = (int(*)[800])lcd_mmap;

  // 计算缩放后的宽度和高度
  int scaledWidth = (int)(infoHeader.biWidth * scale);
  int scaledHeight = (int)(infoHeader.biHeight * scale);

  // 将 BMP 数据转换为 LCD 可用的颜色格式，并将其显示到 LCD 屏幕
  for (int lcdY = y, bmpY = 0;
       lcdY < y + scaledHeight && bmpY < (int)infoHeader.biHeight;
       lcdY++, bmpY = (int)(bmpY + 1 / scale)) {
    for (int lcdX = x, bmpX = 0;
         lcdX < x + scaledWidth && bmpX < (int)infoHeader.biWidth;
         lcdX++, bmpX = (int)(bmpX + 1 / scale)) {
      // 计算 BMP 数据中的像素位置
      int bmpIndex = (infoHeader.biHeight - bmpY - 1) * infoHeader.biWidth *
                         (infoHeader.biBitCount / 8) +
                     bmpX * (infoHeader.biBitCount / 8);

      // 处理 32 位 BMP 图像的颜色通道
      char b = bmpData[bmpIndex + 0];
      char g = bmpData[bmpIndex + 1];
      char r = bmpData[bmpIndex + 2];
      // Alpha 通道值
      char a = bmpData[bmpIndex + 3];

      // 将 32 位颜色数据转换为 24 位
      // 这里只使用 RGB 通道，忽略 Alpha 通道
      lcd[lcdY][lcdX] = b | (g << 8) | (r << 16);
    }
  }

  // 释放资源
  free(bmpData);
  close(fd);
}

/****************************************************************************************************************************************************************
 * @name: fill_screen_with_color
 * @brief: 用指定颜色填充整个屏幕
 * @param {int*} lcd_mmap - 指向 LCD 显示器映射内存的指针
 * @param {color} color - 填充颜色
 * @return: void
 * @note: 该函数将 LCD 屏幕上的所有像素填充为指定的颜色。
 *****************************************************************************************************************************************************************/
void fill_screen_with_color(int* lcd_mmap, color fill_color) {
  for (int y = 0; y < 480; y++) {
    for (int x = 0; x < 800; x++) {
      lcd_mmap[y * 800 + x] = fill_color;
    }
  }
  // 打印详细的屏幕渲染完成信息，包括渲染的颜色值
  printf("屏幕渲染完成，渲染颜色：0x%08X\n", fill_color);
}

/****************************************************************************************************************************************************************
 * @name: show_text
 * @brief: 在屏幕上显示文本
 * @param {int*} lcd_mmap - 指向 LCD 显示器映射内存的指针
 * @param {font*} font - 指向字体结构体的指针
 * @param {const char*} text - 要显示的文本
 * @param {int} fontSize - 字体大小
 * @param {int} textX - 文本的 X 坐标
 * @param {int} textY - 文本的 Y 坐标
 * @param {u32} width - 输出框的宽度
 * @param {u32} height - 输出框的高度
 * @param {color} textColor - 文本的颜色
 * @param {s32} maxWidth - 每行最大宽度
 * @param {int} displayX - 显示的 X 坐标
 * @param {int} displayY - 显示的 Y 坐标
 * @param {color} bgColor - 背景颜色
 * @return: void
 * @note: 该函数负责将指定的文本显示到 LCD
 *上，包括设置字体大小、创建位图、绘制文本以及显示位图。
 *****************************************************************************************************************************************************************/
void show_text(int* lcd_mmap,
               font* font,
               char* text,
               int fontSize,
               int textX,
               int textY,
               u32 width,
               u32 height,
               color textColor,
               s32 maxWidth,
               int displayX,
               int displayY,
               color bgColor) {
  const u32 LCD_WIDTH = 800;
  const u32 LCD_HEIGHT = 480;
  int outOfBounds = 0;

  // 检查 lcd_mmap 是否有效
  if (lcd_mmap == NULL) {
    printf("错误：LCD 映射内存指针无效\n");
    exit(EXIT_FAILURE);
  }

  // 检查绘制区域是否超出 LCD 边界
  if (displayX < 0 || displayY < 0 || displayX + width > LCD_WIDTH ||
      displayY + height > LCD_HEIGHT) {
    printf("错误：绘制区域超出 LCD 边界\n");
    exit(EXIT_FAILURE);
  }

  // 1. 设置字体的大小
  fontSetSize(font, fontSize);

  // 2. 创建一块输出框
  bitmap* bmp = createBitmapWithInit(width, height, 4, bgColor);
  if (bmp == NULL) {
    printf("错误：创建一块输出框失败\n");
    exit(EXIT_FAILURE);
  }

  // 3. 设置文本并绘制到位图上
  fontPrint(font, bmp, textX, textY, text, textColor, maxWidth);

  // 4. 计算 LCD 映射内存的起始地址
  int* lcd_start_ptr = lcd_mmap;

  // 5. 绘制位图到 LCD 上
  for (u32 y = 0; y < height; y++) {
    for (u32 x = 0; x < width; x++) {
      int lcd_x = displayX + x;
      int lcd_y = displayY + y;

      // 检查坐标是否在 LCD 的有效范围内
      if (lcd_x >= 0 && lcd_x < LCD_WIDTH && lcd_y >= 0 && lcd_y < LCD_HEIGHT) {
        u8* pixel = bmp->map + (y * width + x) * 4;

        // 提取 RGB 颜色分量
        char b = pixel[0];
        char g = pixel[1];
        char r = pixel[2];
        // Alpha 通道值（暂未使用）
        char a = pixel[3];
        // 合成颜色值（24 位颜色）
        color c = b | (g << 8) | (r << 16);

        // 确保 lcd_start_ptr 不会越界
        if (lcd_y * LCD_WIDTH + lcd_x >= 0 &&
            lcd_y * LCD_WIDTH + lcd_x < LCD_WIDTH * LCD_HEIGHT) {
          lcd_start_ptr[lcd_y * LCD_WIDTH + lcd_x] = c;
        } else {
          // 标记发生了越界
          outOfBounds = 1;
        }
      } else {
        // 标记发生了越界
        outOfBounds = 1;
      }
    }
  }

  if (outOfBounds) {
    printf("警告：有些像素超出了 LCD 的边界，未被渲染。\n");
  }

  printf("字体 %s 渲染完毕\n", text);

  // 6. 释放资源
  destroyBitmap(bmp);
}

/****************************************************************************************************************************************************************
 * @name: init_font
 * @brief: 初始化字体并加载字体文件
 * @param {char*} fontPath - 字体文件的路径
 * @return: void* - 返回加载的字体对象
 * @note: 该函数尝试加载指定路径的字体文件，如果失败则输出错误信息并退出程序
 *****************************************************************************************************************************************************************/
void* init_font(char* fontPath) {
  void* f = fontLoad(fontPath);
  if (f == NULL) {
    perror("加载字体文件失败");
    exit(EXIT_FAILURE);
  }
  printf("加载字体文件成功\n");
  return f;
}