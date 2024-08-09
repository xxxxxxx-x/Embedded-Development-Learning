/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/demo4/main.c
 * @brief        : 小练习：将两个文件内容合并到一个文件中
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-09 14:15:22
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {

  // 检查参数是否正确
  if (argc != 4) {
    printf("<用法>：%s <要合并的文件1> <要合并的文件2> <合并之后的文件>\n",
           argv[0]);
    exit(EXIT_FAILURE);
  }

  // 打开文件1， 只读模式
  int fd1 = open(argv[1], O_RDONLY);
  // 打开文件2， 只读模式
  int fd2 = open(argv[2], O_RDONLY);
  // 打开或创建文件3， 写模式， 如果存在则截断
  int fd3 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);

  // 检查文件是否成功打开
  if (fd1 < 0 || fd2 < 0 || fd3 < 0) {
    perror("合并失败");
    exit(EXIT_FAILURE);
  }

  char buffer[1024];  // 缓冲区
  ssize_t bytes_read; // 读取到的字节数

  // 读取文件1的内容并写入文件3
  while ((bytes_read = read(fd1, buffer, sizeof(buffer))) > 0) {
    if (write(fd3, buffer, bytes_read) != bytes_read) {
      perror("写入文件时出错");
      close(fd1);
      close(fd2);
      close(fd3);
      exit(EXIT_FAILURE);
    }
  }

  // 检查读取文件1时是否出错
  if (bytes_read < 0) {
    perror("读取文件1时出错");
    close(fd1);
    close(fd2);
    close(fd3);
    exit(EXIT_FAILURE);
  }

  // 读取文件2的内容并写入文件3
  while ((bytes_read = read(fd2, buffer, sizeof(buffer))) > 0) {
    if (write(fd3, buffer, bytes_read) != bytes_read) {
      perror("写入文件时出错");
      close(fd1);
      close(fd2);
      close(fd3);
      exit(EXIT_FAILURE);
    }
  }

  // 检查读取文件2时是否出错
  if (bytes_read < 0) {
    perror("读取文件2时出错");
    close(fd1);
    close(fd2);
    close(fd3);
    exit(EXIT_FAILURE);
  }

  // 关闭所有文件描述符
  close(fd1);
  close(fd2);
  close(fd3);

  printf("文件合并成功\n");
  return 0;
}
