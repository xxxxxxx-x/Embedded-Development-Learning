/****************************************************************************************************************************************************************
 * @file         : /Code/Projects/DineSimple/include/image_display.h
 * @brief        : 图像显示头文件，定义 BMP 文件头和位图信息头结构体及函数原型
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-15 16:16:46
 * @version      : 1.1
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
    unsigned int biSize;         // 位图信息头的大小，通常为40字节
    unsigned int biWidth;        // 图像宽度，以像素为单位
    unsigned int biHeight;       // 图像高度，以像素为单位，正数表示倒置图像
    unsigned short biPlanes;     // 总是设置为1
    unsigned short biBitCount;   // 每个像素的位数，常见值有1、4、8、16、24和32
    unsigned int biCompression;  // 压缩类型，常见值有BI_RGB（无压缩）、BI_RLE8、BI_RLE4、BI_BITFIELDS等
    unsigned int biSizeImage;    // 图像数据的大小，以字节为单位
    unsigned int biXPelsPerMeter;// 水平分辨率，单位为像素/米
    unsigned int biYPelsPerMeter;// 垂直分辨率，单位为像素/米
    unsigned int biClrUsed;      // 实际使用的颜色数，如果为0则默认2的biBitCount次方
    unsigned int biClrImportant; // 重要颜色数，如果为0则所有颜色都重要
} __attribute__((packed)) BITMAPINFOHEADER;

// 函数原型声明
/**
 * @brief 显示图像到 LCD 屏幕
 * 
 * @param lcd_mmap  映射到 LCD 的内存指针
 * @param imagePath 图像文件路径
 * @param x         显示图像的起始 X 坐标
 * @param y         显示图像的起始 Y 坐标
 * @param scale     图像缩放比例
 */
void display_image(int *lcd_mmap, const char *imagePath, int x, int y, double scale);

#endif // IMAGE_DISPLAY_H
