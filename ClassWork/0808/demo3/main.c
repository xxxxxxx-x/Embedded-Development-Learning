/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0808/demo3/main.c
 * @brief        : 小练习：统计一个文件中字符 'a' 的个数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-08 15:24:05
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>  // 包含 open 函数的头文件
#include <stdio.h>  // 包含标准输入输出的头文件
#include <string.h> // 包含字符串处理函数的头文件
#include <unistd.h> // 包含 read 和 close 函数的头文件

int main(int argc, const char *argv[]) {
  // 检查是否提供了文件路径作为命令行参数
  if (argc < 2) {
    fprintf(stderr, "用法: %s <文件路径>\n", argv[0]);
    return 1; // 返回错误代码 1，表示参数不足
  }

  // 打开文件，读取模式
  int file_descriptor = open(argv[1], O_RDONLY);

  // 检查文件是否成功打开
  if (file_descriptor < 0) {
    perror("文件打开失败");
    return 1; // 返回错误代码 1，表示文件打开失败
  }

  // 缓冲区，用于读取文件内容
  char buffer[1024];
  int count = 0; // 计数器，用于统计字符 'a' 的个数

  // 循环读取文件的每个字符
  while (read(file_descriptor, buffer, 1) > 0) {
    // 检查当前字符是否为 'a'
    if (buffer[0] == 'a') {
      count++;
    }
  }

  // 输出字符 'a' 的个数
  printf("文件中字符 'a' 的个数为: %d\n", count);

  // 关闭文件
  close(file_descriptor);

  return 0; // 返回 0，表示程序成功执行
}
