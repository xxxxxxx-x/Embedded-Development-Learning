/****************************************************************************************************************************************************************
 * @file         : /Code/Projects/DineSimple/src/Server/server.c
 * @brief        : 处理触摸事件和事件绑定的实现
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-16 16:27:50
 * @version      : 1.0
 * @note         : 实现了绑定触摸事件、销毁事件和处理触摸事件的函数
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include "../../include/Server/server.h"
#include <stdio.h>
#include <string.h>

/* 初始化全局变量 */
TouchRegion touchRegions[MAX_TOUCH_REGIONS];  // 存储所有触摸区域
int touchRegionCount = 0;                     // 当前触摸区域数量

/****************************************************************************************************************************************************************
 * @name: bind_touch_event
 * @brief: 绑定触摸事件
 * @param {int} start_x - 触摸区域起始 X 坐标
 * @param {int} end_x - 触摸区域结束 X 坐标
 * @param {int} start_y - 触摸区域起始 Y 坐标
 * @param {int} end_y - 触摸区域结束 Y 坐标
 * @param {TouchCallback} callback - 触摸事件回调函数
 * @param {void*} param - 传递给回调函数的参数
 * @return: void
 * @note: 该函数负责绑定触摸事件，使得程序可以响应用户的触摸操作
 *****************************************************************************************************************************************************************/
void bind_touch_event(int start_x,
                      int end_x,
                      int start_y,
                      int end_y,
                      TouchCallback callback,
                      void* param) {
  // 检查触摸区域数量是否超出最大限制
  if (touchRegionCount >= MAX_TOUCH_REGIONS) {
    fprintf(stderr, "触摸区域已达到最大数量\n");
    return;
  }

  // 初始化新的触摸区域
  touchRegions[touchRegionCount].start_x = start_x;
  touchRegions[touchRegionCount].end_x = end_x;
  touchRegions[touchRegionCount].start_y = start_y;
  touchRegions[touchRegionCount].end_y = end_y;
  touchRegions[touchRegionCount].callback = callback;
  touchRegions[touchRegionCount].param = param;  // 存储传递的参数
  touchRegionCount++;

  printf("绑定触摸事件: [%d, %d, %d, %d]\n", start_x, end_x, start_y, end_y);
}

/****************************************************************************************************************************************************************
 * @name: destroy_event
 * @brief: 销毁事件
 * @return: void
 * @note: 该函数负责销毁事件，释放相关资源
 *****************************************************************************************************************************************************************/
void destroy_event(void) {
  // 重置触摸区域计数器和数组内容
  touchRegionCount = 0;
  memset(touchRegions, 0, sizeof(touchRegions));
  printf("所有触摸事件已销毁\n");
}

/****************************************************************************************************************************************************************
 * @name: handle_touch_event
 * @brief: 处理触摸事件
 * @param {int} x - 触摸点的 X 坐标
 * @param {int} y - 触摸点的 Y 坐标
 * @return: void
 * @note: 该函数负责处理触摸事件，根据坐标确定点击的区域，并做出相应处理
 *****************************************************************************************************************************************************************/
void handle_touch_event(int x, int y) {
  // 遍历所有触摸区域，检查触摸点是否在某个区域内
  for (int i = 0; i < touchRegionCount; i++) {
    if (x >= touchRegions[i].start_x && x <= touchRegions[i].end_x &&
        y >= touchRegions[i].start_y && y <= touchRegions[i].end_y) {
      // 如果触摸点在触摸区域内，调用回调函数
      if (touchRegions[i].callback != NULL) {
        printf("触发了触摸事件\n");
        touchRegions[i].callback(touchRegions[i].param);
        return;
      }
    }
  }
}
