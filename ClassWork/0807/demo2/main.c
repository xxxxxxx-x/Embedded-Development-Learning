/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0807/demo2/main.c
 * @brief        : 小练习：利用open接口自己尝试打开一个文件
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-07 17:28:25
 * @version      : 1.1
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
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

  // 使用只读模式打开文件
  int file_descriptor = open(argv[1], O_RDONLY);

  // 检查文件是否打开成功
  if (file_descriptor >= 0) {
    printf("文件打开成功！\n");
    // 在程序结束前关闭文件
    close(file_descriptor);
  } else {
    perror("文件打开失败");
    return 1; // 返回错误代码 1，表示文件打开失败
  }

  return 0; // 返回 0，表示程序成功执行
}
