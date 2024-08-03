#ifndef __TOUCHPANEL_H
#define __TOUCHPANEL_H
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "./font.h"
#include <time.h>
#include <pthread.h>
#include "./FileIOLibrary.h"
#include "./lcd.h"
#include <linux/input.h> //触摸屏必须包含该头文件

// 触摸屏的设备路径，根据实际情况修改即可
#define TS_PATH "/dev/input/event0"

/**
 * @name      ts_init
 * @brief     触摸屏的初始化
 * @param     tspath : 指的是触摸屏的设备路径
 * @date      2024/05/18
 * @version   1.0
 * @note
 * 必须要有:
 *
 */
int ts_init(const char *tspath);
/**
 * @name      GetTouchVal_Once
 * @brief     获取一次触摸屏的按下坐标, 获取有效值为用户松开的位置
 * @param     ts_fd : 指的是触摸屏的文件描述符
 * @param     ts_x  : 指的是用于存储X轴坐标的变量地址
 * @param     ts_y  : 指的是用于存储X轴坐标的变量地址
 * @return
 * @date      2024/05/18
 * @version   1.0 2024年5月18日13:49:02 基本功能实现, 并仅获取松手坐标
 * @note
 * 必须要有:
 *         该函数每次只能获取一次触摸屏坐标，并会把触摸屏的坐标值分别存储到传递进来的两个变量地址下
 *
 *          调用该函数之前，必须提前调用ts_init()对触摸屏进行初始化,否则调用会失败
 *
 */
void GetTouchVal_Once(int ts_fd, int *ts_x, int *ts_y);
#endif