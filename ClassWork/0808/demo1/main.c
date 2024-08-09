/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0808/demo1/main.c
 * @brief        : 小练习：不断打开一个文件，当打开到1023个文件后关闭文件
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-08 11:29:09
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>     // 包含 open 函数的头文件
#include <stdio.h>     // 包含标准输入输出的头文件
#include <sys/stat.h>  // 包含文件状态的头文件
#include <sys/types.h> // 包含文件类型的头文件
#include <unistd.h>    // 包含 close 函数的头文件

int main(int argc, const char *argv[]) {
  // 检查是否提供了文件路径作为命令行参数
  if (argc < 2) {
    fprintf(stderr, "用法: %s <文件路径>\n", argv[0]);
    return 1; // 返回错误代码 1，表示参数不足
  }

  // 文件描述符，用于标识打开的文件
  int file_descriptor;

  // 计数器，用于记录打开文件的次数
  int open_count = 0;

  // 循环打开文件，直到打开的文件数达到 1023 次
  while (open_count < 1023) {
    // 使用只读模式打开文件
    file_descriptor = open(argv[1], O_RDONLY);

    // 检查文件是否打开成功
    if (file_descriptor >= 0) {
      printf("文件描述符 %d: 文件打开成功！\n", file_descriptor);
      open_count++; // 增加计数器

      // 关闭当前打开的文件
      close(file_descriptor);
    } else {
      perror("文件打开失败");
      return 1; // 返回错误代码 1，表示文件打开失败
    }
  }

  // 打开第 1023 个文件
  file_descriptor = open(argv[1], O_RDONLY);

  if (file_descriptor >= 0) {
    printf("文件描述符 %d: 文件打开成功！即将关闭此文件。\n", file_descriptor);
    close(file_descriptor); // 关闭第 1023 个文件
  } else {
    perror("文件打开失败");
    return 1; // 返回错误代码 1，表示文件打开失败
  }

  return 0; // 返回 0，表示程序成功执行
}
