/****************************************************************************************************************************************************************
 * @file         : /DineSimple/include/Tools/tools.h
 * @brief        : 工具函数头文件，包括图像和文本显示功能，以及字体初始化功能
 * @autor        : a_23456@foxmail.com
 * @date         : 2024-08-16 16:34:43
 * @version      : 1.0
 * @note         :
 *包含位图文件头和信息头结构体定义，以及用于显示位图和文本的函数声明
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
 *****************************************************************************************************************************************************************/
#ifndef TOOLS_H
#define TOOLS_H

#include "./font.h"

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

/****************************************************************************************************************************************************************
 * @name: getColor24
 * @brief: 生成一个 24 位颜色值，仅包含 RGB 分量
 * @param {u8} R -  红色分量
 * @param {u8} G -  绿色分量
 * @param {u8} B -  蓝色分量
 * @return: color - 组合后的 24 位颜色值
 * @date: 2024-08-17
 * @version: 1.0
 * @note: 通过位移和按位或操作，将各个颜色分量组合成一个 24 位颜色值
 *****************************************************************************************************************************************************************/
#define getColor24(R, G, B) (B << 8 | G << 16 | R << 24)

/****************************************************************************************************************************************************************
 * @name: show_bitmap
 * @brief: 从图像文件中读取数据并缩放显示到 LCD 屏幕
 * @param {int*} lcd_mmap - 映射到 LCD 的内存指针
 * @param {const char*} imagePath - 图像文件路径
 * @param {int} x - 显示图像的起始 X 坐标
 * @param {int} y - 显示图像的起始 Y 坐标
 * @param {double} scale - 图像缩放比例
 * @return: void
 * @note: 该函数读取指定路径的图像文件，将其缩放后显示在 LCD 屏幕的指定位置。
 *****************************************************************************************************************************************************************/
void show_bitmap(int *lcd_mmap, const char *imagePath, int x, int y,
                 double scale);

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
void show_text(int *lcd_mmap, font *font, char *text, int fontSize, int textX,
               int textY, u32 width, u32 height, color textColor, s32 maxWidth,
               int displayX, int displayY, color bgColor);

/****************************************************************************************************************************************************************
 * @name: fill_screen_with_color
 * @brief: 用指定颜色填充整个屏幕
 * @param {int*} lcd_mmap - 指向 LCD 显示器映射内存的指针
 * @param {color} color - 填充颜色
 * @return: void
 * @note: 该函数将 LCD 屏幕上的所有像素填充为指定的颜色。
 *****************************************************************************************************************************************************************/
void fill_screen_with_color(int *lcd_mmap, color color);

/****************************************************************************************************************************************************************
 * @name: init_font
 * @brief: 初始化字体
 * @param {char*} fontPath - 字体文件路径
 * @return: void* - 返回字体结构体指针
 * @note: 该函数加载指定路径的字体文件，并返回字体结构体指针，供后续使用
 *****************************************************************************************************************************************************************/
void *init_font(char *fontPath);

#endif // TOOLS_H
