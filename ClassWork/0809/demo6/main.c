/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/demo6/main.c
 * @brief        : 小练习：读取并输出 BMP 文件的头部信息
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-09 16:00:00
 * @version      : 1.0
 * @note         : 需要指定 BMP 文件路径作为命令行参数
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char *argv[]) {
  // 检查命令行参数是否正确
  if (argc != 2) {
    fprintf(stderr, "用法: %s <bmp文件路径>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // 打开 BMP 图片文件
  int fileDescriptor = open(argv[1], O_RDONLY);
  if (fileDescriptor < 0) {
    perror("打开文件失败");
    return EXIT_FAILURE;
  }

  // 读取文件头
  BITMAPFILEHEADER fileHeader;
  if (read(fileDescriptor, &fileHeader, sizeof(BITMAPFILEHEADER)) !=
      sizeof(BITMAPFILEHEADER)) {
    perror("读取文件头失败");
    close(fileDescriptor);
    return EXIT_FAILURE;
  }

  // 读取位图信息头
  BITMAPINFOHEADER infoHeader;
  if (read(fileDescriptor, &infoHeader, sizeof(BITMAPINFOHEADER)) !=
      sizeof(BITMAPINFOHEADER)) {
    perror("读取位图信息头失败");
    close(fileDescriptor);
    return EXIT_FAILURE;
  }

  // 输出 BMP 文件头信息
  printf("BMP 文件大小: %u 字节\n", fileHeader.bfSize);
  printf("图像宽度: %u 像素\n", infoHeader.biWidth);
  printf("图像高度: %u 像素\n", infoHeader.biHeight);
  printf("色深: %u 位\n", infoHeader.biBitCount);

  // 关闭文件
  close(fileDescriptor);
  return EXIT_SUCCESS;
}
