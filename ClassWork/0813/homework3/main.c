/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0813/homework3/main.c
 * @brief        : 作业：把指定目录下的所有bmp图片文件都显示一次
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-13 21:05:52
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// 定义 BMP 文件头结构体
typedef struct {
  unsigned short bfType;       // 2 字节，文件类型
  unsigned int bfSize;         // 4 字节，文件大小
  unsigned short bfReserved1;  // 2 字节，保留
  unsigned short bfReserved2;  // 2 字节，保留
  unsigned int bfOffBits;      // 4 字节，图像数据偏移量
} __attribute__((packed)) BITMAPFILEHEADER;

// 定义 BMP 位图信息头结构体
typedef struct {
  unsigned int biSize;    // 4 字节，结构体大小，通常是40字节
  unsigned int biWidth;   // 4 字节，图像宽度，单位是像素
  unsigned int biHeight;  // 4 字节，图像高度，单位是像素
  unsigned short biPlanes;  // 2 字节，目标设备的平面数，总是设置为1
  unsigned short biBitCount;   // 2 字节，每个像素的位数
  unsigned int biCompression;  // 4 字节，压缩类型
  unsigned int biSizeImage;    // 4 字节，图像大小，以字节为单位
  unsigned int biXPelsPerMeter;  // 4 字节，水平分辨率，单位是像素/米
  unsigned int biYPelsPerMeter;  // 4 字节，垂直分辨率，单位是像素/米
  unsigned int biClrUsed;        // 4 字节，实际使用的颜色数
  unsigned int biClrImportant;  // 4 字节，重要颜色数
} __attribute__((packed)) BITMAPINFOHEADER;

// 全局变量，LCD 映射地址
int (*lcd)[800] = NULL;

// 映射 LCD 设备
void* mmap_lcd() {
  // 打开 LCD 设备
  int fd = open("/dev/fb0", O_RDWR);
  if (fd < 0) {
    perror("打开 LCD 失败");
    return NULL;
  }

  // 映射 LCD 设备
  void* p =
      mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (p == MAP_FAILED) {
    perror("映射失败");
    close(fd);
    return NULL;
  } else {
    printf("映射 LCD 成功\n");
  }

  close(fd);
  return p;
}

// 显示一张 24 位的 BMP 图像
void show_24_bmp(const char* path) {
  // 打开 BMP 图片文件
  int fd = open(path, O_RDWR);
  if (fd < 0) {
    perror("打开文件失败");
    return;
  }

  // 读取文件头
  BITMAPFILEHEADER fileHeader;
  read(fd, &fileHeader, sizeof(BITMAPFILEHEADER));

  // 读取文件信息头
  BITMAPINFOHEADER infoHeader;
  read(fd, &infoHeader, sizeof(BITMAPINFOHEADER));

  printf("显示 BMP 文件: %s\n文件大小: %d 字节\n宽度: %d\n高度: %d\n色深: %d\n",
         path, fileHeader.bfSize, infoHeader.biWidth, infoHeader.biHeight,
         infoHeader.biBitCount);

  // 读取图像的像素数据
  int imageSize =
      infoHeader.biWidth * infoHeader.biHeight * infoHeader.biBitCount / 8;
  char* imageData = (char*)malloc(imageSize);
  read(fd, imageData, imageSize);

  // 将 char 类型的 RGB 数据转换为 int 类型的颜色数据
  int color[infoHeader.biHeight][infoHeader.biWidth];
  char* p = imageData;
  for (int y = 0; y < infoHeader.biHeight; y++) {
    for (int x = 0; x < infoHeader.biWidth; x++) {
      char b = *p++;
      char g = *p++;
      char r = *p++;
      color[y][x] = b | g << 8 | r << 16 | 0 << 24;
    }
  }

  // 将颜色数据写入到 LCD 映射地址
  for (int y = 0; y < infoHeader.biHeight; y++) {
    for (int x = 0; x < infoHeader.biWidth; x++) {
      lcd[y][x] = color[infoHeader.biHeight - 1 - y][x];  // 翻转显示
    }
  }

  // 释放分配的内存并关闭文件
  free(imageData);
  close(fd);
}

// 搜索指定目录下的 BMP 文件并显示
void searchBmpFiles(const char* dirPath) {
  DIR* dp = opendir(dirPath);
  if (!dp) {
    perror("打开目录失败");
    return;
  }

  struct dirent* entry;
  while ((entry = readdir(dp)) != NULL) {
    int nameLen = strlen(entry->d_name);
    char filePath[strlen(dirPath) + nameLen + 2];
    sprintf(filePath, "%s/%s", dirPath, entry->d_name);

    if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
      // 递归搜索子目录
      searchBmpFiles(filePath);
    } else if (entry->d_type == DT_REG && nameLen > 4 &&
               strcmp(entry->d_name + nameLen - 4, ".bmp") == 0) {
      // 如果是 BMP 文件，显示图片
      show_24_bmp(filePath);
      sleep(1);  // 暂停 1 秒钟
    }
  }

  closedir(dp);
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    printf("用法：%s <目录路径>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 映射 LCD 设备
  lcd = mmap_lcd();
  if (lcd == NULL) {
    exit(EXIT_FAILURE);
  }

  // 搜索并显示 BMP 文件
  searchBmpFiles(argv[1]);

  return 0;
}
