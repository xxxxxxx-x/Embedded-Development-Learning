/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0808/demo2/main.c
 * @brief        : 小练习: 把自己的姓名，学号，电话写入到一个文件中
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-08 14:24:18
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <fcntl.h>     // 包含 open 函数的头文件
#include <stdio.h>     // 包含标准输入输出的头文件
#include <string.h>    // 包含字符串处理函数的头文件
#include <sys/stat.h>  // 包含文件状态的头文件
#include <sys/types.h> // 包含文件类型的头文件
#include <unistd.h>    // 包含 close 函数的头文件

int main(int argc, const char *argv[]) {
  // 检查是否提供了文件路径作为命令行参数
  if (argc < 2) {
    fprintf(stderr, "用法: %s <文件路径>\n", argv[0]);
    return 1; // 返回错误代码 1，表示参数不足
  }

  // 打开文件，读写模式。如果文件不存在，则创建该文件
  int file_descriptor = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  // 检查文件是否成功打开
  if (file_descriptor < 0) {
    perror("文件打开失败");
    return 1; // 返回错误代码 1，表示文件打开失败
  }

  // 要写入文件的信息
  const char *name = "姓名: 张三\n";
  const char *student_id = "学号: 2022012345\n";
  const char *phone = "电话: 123-456-7890\n";

  // 将信息写入文件
  ssize_t bytes_written;

  bytes_written = write(file_descriptor, name, strlen(name));
  if (bytes_written < 0) {
    perror("写入姓名失败");
    close(file_descriptor); // 关闭文件
    return 1;               // 返回错误代码 1，表示写入失败
  }

  bytes_written = write(file_descriptor, student_id, strlen(student_id));
  if (bytes_written < 0) {
    perror("写入学号失败");
    close(file_descriptor); // 关闭文件
    return 1;               // 返回错误代码 1，表示写入失败
  }

  bytes_written = write(file_descriptor, phone, strlen(phone));
  if (bytes_written < 0) {
    perror("写入电话失败");
    close(file_descriptor); // 关闭文件
    return 1;               // 返回错误代码 1，表示写入失败
  }

  // 成功写入提示
  printf("成功将姓名、学号和电话写入文件 %s\n", argv[1]);

  // 关闭文件
  close(file_descriptor);

  return 0; // 返回 0，表示程序成功执行
}
