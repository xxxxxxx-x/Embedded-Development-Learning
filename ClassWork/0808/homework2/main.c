/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0808/homework2/main.c
 * @brief        :
 *                 作业：把用户从键盘输入的数据写入到一个文件中，当用户输入#时，结束获取，并输出文件的所有数据
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-08 20:19:37
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>   // 包含 open 函数的头文件
#include <stdio.h>   // 包含标准输入输出的头文件
#include <string.h>  // 包含字符串处理函数的头文件
#include <unistd.h>  // 包含系统调用函数的头文件

int main(int argc, const char* argv[]) {
  // 定义存储用户输入的缓冲区
  char input_buffer[1024];

  // 打开文件，使用 O_RDWR 模式
  int fd = open("output.txt", O_RDWR);
  if (fd == -1) {
    perror("无法打开文件");
    return 1;
  }

  // 循环读取用户输入，直到输入 '#' 结束
  printf("请输入内容（输入 '#' 结束）：");

  while (1) {
    fgets(input_buffer, sizeof(input_buffer), stdin);

    // 检查是否输入了 '#'
    if (strstr(input_buffer, "#") != NULL) {
      break;
    }

    // 将用户输入写入文件
    write(fd, input_buffer, strlen(input_buffer));
  }

  // 关闭文件
  close(fd);
  return 0;
}
