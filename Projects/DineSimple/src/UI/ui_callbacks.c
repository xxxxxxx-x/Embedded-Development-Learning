/****************************************************************************************************************************************************************
 * @file         : /DineSimple/src/UI/ui_callbacks.c
 * @brief        : 回调函数实现
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-16 21:09:13
 * @version      : 1.0
 * @note         : 实现了与 UI 相关的回调函数
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include "../../include/UI/ui_callbacks.h"
#include <stdio.h>
#include "../../include/Tasks/tasks.h"

/****************************************************************************************************************************************************************
 * @name: welcome_start_order_callback
 * @brief: 处理开始点餐按钮的回调函数
 * @param {void*} arg - 触摸事件数据
 * @return: void
 * @note: 该函数在点击开始点餐按钮时被调用
 *****************************************************************************************************************************************************************/
void welcome_start_order_callback(void* arg) {
  UI* ui = (UI*)arg;

  printf("欢迎页-开始点餐按钮回调函数正在执行\n");

  // 加锁，进入临界区
  pthread_mutex_lock(&screen_mutex);

  // 更新当前屏幕状态
  ui->currentScreen = MENU_SCREEN;

  // 通知等待的线程条件变量的状态已改变
  pthread_cond_signal(&screen_cond);

  // 解锁，离开临界区
  pthread_mutex_unlock(&screen_mutex);
}

/****************************************************************************************************************************************************************
 * @name: welcome_user_login_callback
 * @brief: 处理用户登录按钮的回调函数
 * @param {void*} arg - 触摸事件数据
 * @return: void
 * @note: 该函数在点击用户登录按钮时被调用
 *****************************************************************************************************************************************************************/
void welcome_user_login_callback(void* arg) {
  printf("用户登录 按钮被点击\n");
  // 这里可以添加处理用户登录逻辑的代码
}

/****************************************************************************************************************************************************************
 * @name: welcome_shop_login_callback
 * @brief: 处理商家登录按钮的回调函数
 * @param {void*} arg - 触摸事件数据
 * @return: void
 * @note: 该函数在点击商家登录按钮时被调用
 *****************************************************************************************************************************************************************/
void welcome_shop_login_callback(void* arg) {
  printf("商家登录 按钮被点击\n");
  // 这里可以添加处理商家登录逻辑的代码
}
