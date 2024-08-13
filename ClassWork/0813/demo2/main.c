/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0813/demo2/main.c
 * @brief        : 小练习：读取一个目录中的所有文件名
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-13 14:41:23
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
  // 打开目录
  DIR *dp = opendir("../demo2");
  if (!dp) {
    perror("打开目录失败");
    exit(EXIT_FAILURE);
  } else {
    printf("打开目录成功！\n");
  }

  struct dirent *entry = readdir(dp);

  if (!entry) {
    perror("读取目录失败");
    closedir(dp);
    exit(EXIT_FAILURE);

  } else {
    printf("读取目录成功！\n");
  }

  // 读取目录中的每个文件
  while ((entry = readdir(dp)) != NULL) {
    printf("文件类型 = %d, 文件名 = %s\n", entry->d_type, entry->d_name);
  }

  // 关闭目录
  closedir(dp);

  return 0;
}
