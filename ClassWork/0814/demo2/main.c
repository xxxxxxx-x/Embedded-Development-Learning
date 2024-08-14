/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0814/demo2/main.c
 * @brief        :
 *                 小练习：触摸屏输入处理程序，修复滑动越界问题并添加按钮状态判断
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

// 屏幕分辨率常量
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

// 主函数
int main(int argc, const char* argv[]) {
  // 打开触摸屏设备文件
  int device_fd = open("/dev/input/event0", O_RDWR);
  if (device_fd < 0) {
    perror("打开触摸屏失败");
    return -1;
  }

  // 无限循环读取触摸屏输入事件
  while (1) {
    struct input_event input_event;

    // 读取输入事件到 input_event 结构体中
    ssize_t bytes_read = read(device_fd, &input_event, sizeof(input_event));
    if (bytes_read < 0) {
      perror("读取输入事件失败");
      close(device_fd);
      return -1;
    }

    // 处理 Y 轴触摸事件
    if (input_event.type == EV_ABS && input_event.code == ABS_Y) {
      // 将原始值转换为屏幕坐标
      int y_position = SCREEN_HEIGHT * input_event.value / 600;
      printf("y=%d\n", y_position);
    }

    // 处理 X 轴触摸事件
    if (input_event.type == EV_ABS && input_event.code == ABS_X) {
      // 将原始值转换为屏幕坐标
      int x_position = SCREEN_WIDTH * input_event.value / 1024;
      printf("x=%d\n", x_position);
    }

    // 处理按键事件
    if (input_event.type == EV_KEY) {
      if (input_event.value == 1) {
        printf("触摸屏按下\n");
      } else if (input_event.value == 0) {
        printf("触摸屏松开\n");
      }
    }
  }

  // 关闭设备文件
  close(device_fd);
  return 0;
}
