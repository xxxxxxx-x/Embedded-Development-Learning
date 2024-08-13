/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0813/demo1/main.c
 * @brief        : 小练习：尝试打开linux系统下的一个目录
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-13 14:16:20
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>

#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[]) {

  DIR *dp = opendir("../demo1");

  if (dp == NULL) {
    perror("打开目录失败!");
    exit(EXIT_FAILURE);
  } else {
    printf("打开目录成功！\n");
  }

  return 0;
}