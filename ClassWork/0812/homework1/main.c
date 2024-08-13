/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0812/homework1/main.c
 * @brief        : 作业：利用字节流操作，统计一个文件中的字母 与 数字的 数量.
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 18:49:09
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

int main(int argc, const char *argv[]) {
  FILE *file = fopen("my.txt", "r");
  if (file == NULL) {
    perror("无法打开文件");
    return 1;
  }

  int Letters = 0, Numbers = 0;
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      Letters++;
    }

    if (ch >= '0' && ch <= '9') {
      Numbers++;
    }
  }

  fclose(file);

  printf("统计结果：该文件中共有字母%d个，数字%d个\n", Letters, Numbers);
  return 0;
}
