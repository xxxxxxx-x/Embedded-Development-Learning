/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0814/demo3/main.c
 * @brief        : 小练习：使用枚举封装上下左右滑动的算法设计
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-14 14:17:27
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 屏幕分辨率常量
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

// 枚举类型定义滑动方向
typedef enum { NONE, UP, DOWN, LEFT, RIGHT } SwipeDirection;

// 获取滑动方向的函数
SwipeDirection get_swipe_direction(int start_x,
                                   int start_y,
                                   int end_x,
                                   int end_y) {
  int delta_x = end_x - start_x;
  int delta_y = end_y - start_y;

  if (abs(delta_x) > abs(delta_y)) {
    if (delta_x > 0)
      return RIGHT;
    else
      return LEFT;
  } else {
    if (delta_y > 0)
      return DOWN;
    else
      return UP;
  }
}

// 打印滑动方向
void print_swipe_direction(SwipeDirection direction) {
  switch (direction) {
    case UP:
      printf("上滑\n");
      break;
    case DOWN:
      printf("下滑\n");
      break;
    case LEFT:
      printf("左滑\n");
      break;
    case RIGHT:
      printf("右滑\n");
      break;
    default:
      break;
  }
}

// 主函数
int main(int argc, const char* argv[]) {
  // 打开触摸屏设备文件
  int device_fd = open("/dev/input/event0", O_RDWR);
  if (device_fd < 0) {
    perror("打开触摸屏失败");
    return -1;
  }

  int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
  int touching = 0;  // 触摸状态标志

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
      int y_position =
          SCREEN_HEIGHT * input_event.value / 600;  // 将原始值转换为屏幕坐标
      if (touching) {
        end_y = y_position;  // 更新结束位置
      } else {
        start_y = y_position;  // 更新开始位置
      }
      printf("y=%d\n", y_position);
    }

    // 处理 X 轴触摸事件
    if (input_event.type == EV_ABS && input_event.code == ABS_X) {
      int x_position =
          SCREEN_WIDTH * input_event.value / 1024;  // 将原始值转换为屏幕坐标
      if (touching) {
        end_x = x_position;  // 更新结束位置
      } else {
        start_x = x_position;  // 更新开始位置
      }
      printf("x=%d\n", x_position);
    }

    // 处理按键事件
    if (input_event.type == EV_KEY) {
      if (input_event.code == BTN_TOUCH) {  // 检测触摸事件
        if (input_event.value == 1) {
          touching = 1;  // 触摸开始
          printf("触摸屏按下\n");
        } else if (input_event.value == 0) {
          touching = 0;  // 触摸结束
          printf("触摸屏松开\n");
          SwipeDirection direction = get_swipe_direction(
              start_x, start_y, end_x, end_y);  // 获取滑动方向
          print_swipe_direction(direction);     // 打印滑动方向
        }
      }
    }
  }

  // 关闭设备文件
  close(device_fd);
  return 0;
}
