/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0812/demo2/main.c
 * @brief        : 小练习：写入一个字符串数组到文件中
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 14:22:16
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  // 要写入文件的字符串数组
  char buf[] = "helloworld!\n";

  // 打开文件进行写操作
  FILE *fp = fopen("my.txt", "w");
  if (fp == NULL) {
    perror("打开文件失败");
    return -1;
  }

  // 将字符串数组写入文件
  size_t len = strlen(buf);                 // 获取字符串长度
  size_t written = fwrite(buf, 1, len, fp); // 写入文件

  // 检查是否成功写入所有数据
  if (written != len) {
    perror("写入文件失败");
    fclose(fp);
    return -1;
  }

  // 关闭文件
  fclose(fp);

  printf("成功写入字符串到文件中。\n");

  return 0;
}
