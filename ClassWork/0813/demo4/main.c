/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0813/demo4/main.c
 * @brief        : 小练习：将文件权限（如777）转为 rwxrwxrwx 的显示格式
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-13 16:35:40
 * @version      : 1.5
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

// 打印文件权限的函数
void printPermissions(mode_t mode) {
  // 权限字符数组，表示执行、写、读权限
  const char *permissions = "xwr";

  // 权限位掩码，分别对应用户、用户组和其他用户的权限
  for (int i = 2; i >= 0; i--) {
    // 打印每组用户的权限
    for (int j = 2; j >= 0; j--) {
      // (i * 3 + j) 计算出当前权限位的位置
      // 用户的权限位为0-2，用户组的为3-5，其他用户的为6-8
      if (mode & (1 << (i * 3 + j))) {
        printf("%c", permissions[j]);
      } else {
        printf("-");
      }
    }
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  // 定义一个文件信息结构体
  struct stat file_stat;

  // 判断输入的参数数量是否正确
  if (argc != 2) {
    fprintf(stderr, "用法: %s <文件路径>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 获取文件的状态信息
  if (lstat(argv[1], &file_stat) == -1) {
    perror("获取文件状态失败");
    exit(EXIT_FAILURE);
  }

  // 打印权限信息
  printPermissions(file_stat.st_mode);

  return 0;
}
