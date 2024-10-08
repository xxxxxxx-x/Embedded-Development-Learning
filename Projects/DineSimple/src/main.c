/****************************************************************************************************************************************************************
 * @file         : /DineSimple/src/main.c
 * @brief        : 主程序文件，负责设备初始化和图像显示
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-15 19:16:37
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../include/Device/device.h"
#include "../include/Tasks/tasks.h"
#include "../include/Tools/tools.h"
#include "../include/UI/ui.h"
#include "../include/UI/ui_tasks.h"

// 全局 UI 组件
UI ui;

int main(int argc, const char* argv[]) {
  /*********初始化*********/
  ui.lcd_mmap = init_lcd();       // 初始化 LCD 设备
  ui.input = init_touchscreen();  // 初始化触摸屏设备
  ui.font = init_font( "./resources/fonts/simfang.ttf");  //初始化字体文件
  ui.currentScreen = WELCOME_SCREEN;  // 初始状态为欢迎页
  ui.previousScreen = EXIT;
  /*********END*********/

  /*********创建线程*********/
  create_thread(ts_task, &ui, "ts_task", 0);
  create_thread(ts_get_time, &ui, "ts_get_time", 0);
  /*********END*********/

  /*********显示开机动画*********/
  // show_boot_animation(&ui, "./resources/animations/boot_logo", 231);
  /*********END*********/

  /*********测试*********/

  /*********END*********/

  /*********状态切换*********/

  while (1) {
    pthread_mutex_lock(&screen_mutex);

    while (ui.currentScreen == ui.previousScreen) {
      pthread_cond_wait(&screen_cond, &screen_mutex);  // 阻塞主线程直到状态改变
    }

    switch (ui.currentScreen) {
      case WELCOME_SCREEN:
        show_WelcomeScreen(&ui);
        break;
      case USER_LOGIN_SCREEN:
        show_UserLoginScreen(&ui);
        break;
      case MENU_SCREEN:
        show_MenuScreen(&ui);
        break;
      case SHOPPING_CART_SCREEN:
        show_ShoppingCartScreen(&ui);
        break;
      case ORDER_HISTORY_SCREEN:
        show_OrderHistoryScreen(&ui);
        break;
      case EXIT:
        handle_Exit(&ui);
        break;
      default:
        printf("未知屏幕\n");
        break;
    }

    ui.previousScreen = ui.currentScreen;
    pthread_mutex_unlock(&screen_mutex);
    printf("状态发生改变！\n");
  }
  /*********END*********/

  return 0;
}
