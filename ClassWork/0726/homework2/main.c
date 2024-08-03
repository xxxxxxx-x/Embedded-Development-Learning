/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0726/homework2/main.c
 * @brief        :
 *                 学生管理系统的主程序，提供添加、显示、查找、修改和删除学生信息的功能
 * @details      : 用户可以通过菜单选择相应功能，对学生信息进行管理
 * @note         : 所有操作均需在有效输入的情况下进行
 * @version      : 1.0
 * @date         : 2024-07-26 20:58:32
 * @author       : a_23456@foxmail.com
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include "student.h" // 包含学生管理模块的头文件
#include "tools.h"   // 包含工具函数模块的头文件
#include <setjmp.h>

jmp_buf quit_env, back_env;

int main(int argc, const char *argv[]) {

  while (true) {
    system("clear");
    // 打印主菜单，供用户选择操作
    printf("欢迎使用学生管理系统\n");
    printf("请选择功能：(1.添加学生信息  2.显示学生信息  3.查找学生  "
           "4.修改学生信息  5.删除学生信息  0.退出):");

    // 获取用户输入的菜单选项
    char key = inputCharacters();

    // 根据用户输入的选项执行相应的功能
    switch (key) {
    case '1': {
      addStudent(); // 添加学生信息
      break;
    }
    case '2': {
      showStudent(); // 显示学生信息
      break;
    }
    case '3': {
      findStudent(); // 查找学生信息
      break;
    }
    case '4': {
      modifyStudent(); // 修改学生信息
      break;
    }
    case '5': {
      deleteStudent(); // 删除学生信息
      break;
    }
    case '0': {
      // 退出程序
      printf("您确定要退出吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        break;
      }
      printf("期待您的下次使用，再见！\n");
      exit(0);
      break;
    }
    default: {
      // 处理无效输入
      printf("未知指令，请输入任意键继续！\n");
      pauseWait();
    }
    }
  }

  return 0;
}
