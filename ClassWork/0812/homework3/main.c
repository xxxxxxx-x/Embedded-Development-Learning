/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0812/homework3/main.c
 * @brief        :
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 19:20:58
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
int main(int argc, const char *argv[]) {

  if (argc != 3) {
    printf("<用法>:%s <源文件> <新文件>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *srcFile = fopen(argv[1], "r");
  FILE *newFile = fopen(argv[2], "w");

  if (srcFile == NULL || newFile == NULL) {
    perror("无法打开文件");
    exit(EXIT_FAILURE);
  }

  int ch;
  while ((ch = fgetc(srcFile)) != EOF) {
    fputc(ch, newFile);
  }

  fclose(srcFile);
  fclose(newFile);

  return 0;
}