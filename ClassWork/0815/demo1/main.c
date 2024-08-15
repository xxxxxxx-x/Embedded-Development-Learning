#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

// 添加字库的头文件
#include "font.h"

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

void show_font_to_lcd(unsigned int* p, int px, int py, bitmap* bm) {
  // 检查参数是否有效
  if (p == NULL || bm == NULL) {
    fprintf(stderr, "Invalid parameters\n");
    return;
  }

  // 获取bitmap的宽度和高度
  u32 width = bm->width;
  u32 height = bm->height;

  // 获取bitmap的字节数
  u32 byteperpixel = bm->byteperpixel;

  // 映射首地址指针
  unsigned int* lcd_ptr = p + py * 800 + px;  // 假设LCD分辨率为800x480

  // 遍历bitmap中的每个像素
  for (u32 y = 0; y < height; y++) {
    for (u32 x = 0; x < width; x++) {
      // 计算bitmap中像素的位置
      u8* pixel = bm->map + (y * width + x) * byteperpixel;

      // 计算颜色值
      color c = getColor(pixel[3], pixel[2], pixel[1], pixel[0]);

      // 设置LCD上的像素颜色
      lcd_ptr[y * 800 + x] = c;
    }
  }
}

int main() {
  // 1.映射LCD 设备
  lcd = mmap_lcd();

  // 2.加载字库
  font* f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");

  // 3.设置字体的大小
  fontSetSize(f, 64);

  // 4.创建一块输出框
  // 设置竖屏显示
  bitmap* b = createBitmap(100, 400, 4);
  // 设置横向满100个像素就换行
  fontPrint(f, b, 0, 0, "四 季 平 安", getColor(255, 0, 0, 0), 100);
  show_font_to_lcd((void*)lcd, 600, 0, b);
}