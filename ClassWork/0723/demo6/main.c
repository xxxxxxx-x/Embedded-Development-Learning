/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0723/demo6/main.c
 * @brief        : 小练习：利用字符串strtok把一个路径的所有文件名分割出来
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-23 16:53:37
 * @version      : 1.1
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  // 定义路径字符串，长度足够存储路径及终止符
  char path[] = "/home/user/Code/ClassWork/0723/demo5";

  // 使用strtok函数分割路径中的每个文件夹或文件名
  char *token = strtok(path, "/");

  // 遍历分割得到的每个部分
  while (token != NULL) {
    printf("%s\n", token);     // 打印当前分割出的部分
    token = strtok(NULL, "/"); // 继续分割剩余的字符串
  }

  return 0;
}
