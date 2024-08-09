/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/demo2/main.c
 * @brief        : 小练习：读取指定文件的内容并输出到标准输出
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-09 10:50:02
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

  // 打开文件，O_RDONLY表示只读模式
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("无法打开文件");
    exit(EXIT_FAILURE);
  }

  ssize_t bytesRead;
  char buffer[1024];
  printf("文件内容为：\n");

  // 读取文件内容并输出
  while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
    // 使用write函数直接将读取的数据写到标准输出
    if (write(STDOUT_FILENO, buffer, bytesRead) != bytesRead) {
      perror("写入标准输出失败");
      close(fd);
      exit(EXIT_FAILURE);
    }
  }

  // 检查读取过程中是否发生了错误
  if (bytesRead < 0) {
    perror("读取文件失败");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // 关闭文件描述符
  close(fd);
  return 0;
}
