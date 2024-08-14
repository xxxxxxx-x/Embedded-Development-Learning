/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0814/demo1/main.c
 * @brief        : 小练习：触摸屏输入处理程序
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-14 10:48:37
 * @version      : 1.0
 * @note         : 该程序读取触摸屏的输入事件并输出触摸坐标
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

// 主函数
int main(int argc, const char* argv[]) {
  // 打开触摸屏设备文件
  int fd = open("/dev/input/event0", O_RDWR);
  if (fd < 0) {
    perror("打开触摸屏失败");
    return -1;
  }

  // 无限循环读取触摸屏输入事件
  while (1) {
    struct input_event ev;

    // 读取输入事件到 ev 结构体中
    ssize_t bytesRead = read(fd, &ev, sizeof(ev));
    if (bytesRead < 0) {
      perror("读取输入事件失败");
      close(fd);
      return -1;
    }

    // 打印事件类型、事件代码和值
    // printf("ev.type=%d  ev.code=%d  ev.value=%d\n", ev.type, ev.code, ev.value);

    // 处理 Y 轴触摸事件
    if (ev.type == EV_ABS && ev.code == ABS_Y) {
      printf("y=%d\n", ev.value);
    }

    // 处理 X 轴触摸事件
    if (ev.type == EV_ABS && ev.code == ABS_X) {
      printf("x=%d\n", ev.value);
    }
  }

  // 关闭设备文件
  close(fd);
  return 0;
}
