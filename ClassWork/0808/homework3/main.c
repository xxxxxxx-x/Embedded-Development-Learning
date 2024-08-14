/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0808/homework3/main.c
 * @brief        : 作业：把一个文件中的大写字母获取出来，写入到另外一个文件中。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-08 20:43:23
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <ctype.h>   // 包含字符处理函数的头文件
#include <fcntl.h>   // 包含 open 函数的头文件
#include <stdio.h>   // 包含标准输入输出的头文件
#include <unistd.h>  // 包含系统调用函数的头文件

int main(int argc, const char* argv[]) {
  // 打开读取文件（只读模式）和写入文件（写入模式）
  int read_fd = open("read.txt", O_RDONLY);
  int write_fd = open("write.txt", O_WRONLY);

  // 检查文件是否成功打开
  if (read_fd == -1 || write_fd == -1) {
    perror("无法打开文件");
    return 1;
  }

  // 定义缓冲区用于读取文件内容
  char buffer[1024];
  ssize_t bytes_read;

  // 循环读取文件内容并提取大写字母
  while ((bytes_read = read(read_fd, buffer, sizeof(buffer))) > 0) {
    for (ssize_t i = 0; i < bytes_read; ++i) {
      // 检查字符是否为大写字母
      if (isupper((unsigned char)buffer[i])) {
        // 写入大写字母到目标文件
        if (write(write_fd, &buffer[i], 1) == -1) {
          perror("写入文件失败");
          close(read_fd);
          close(write_fd);
          return 1;
        }
      }
    }
  }

  // 检查读取文件过程中是否发生错误
  if (bytes_read == -1) {
    perror("读取文件失败");
  }

  // 关闭文件
  close(read_fd);
  close(write_fd);

  return 0;
}
