/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/demo3/main.c
 * @brief        : 小练习：实现一个简单的 mytouch 程序，用于创建文件
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-09 11:29:41
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>  // 包含文件控制选项的头文件
#include <stdio.h>  // 包含标准输入输出的头文件
#include <stdlib.h> // 包含标准库函数的头文件
#include <unistd.h> // 包含POSIX操作系统API的头文件

int main(int argc, const char *argv[]) {
  // 检查命令行参数数量是否正确
  if (argc != 2) {
    fprintf(stderr, "用法: %s <文件路径>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 尝试创建文件，O_EXCL表示文件如果已存在则报错，O_CREAT表示如果文件不存在则创建，O_WRONLY表示只写模式
  int fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644);
  if (fd < 0) {
    perror("文件创建失败");
    exit(EXIT_FAILURE);
  }

  // 关闭文件描述符
  close(fd);
  return 0;
}
