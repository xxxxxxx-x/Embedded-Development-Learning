/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0812/homework4/main.c
 * @brief        : 作业：利用标准IO实现翻转一个文件中的内容
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 19:30:28
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
int main(int argc, const char *argv[]) {

  FILE *readfile = fopen("my.txt", "r");
  FILE *writefile = fopen("my.txt", "r+");

  if (readfile == NULL || writefile == NULL) {
    perror("无法打开文件");
    exit(EXIT_FAILURE);
  }

  fseek(readfile, 0, SEEK_END);
  long filesize = ftell(readfile);
  printf("%ld\n", filesize);

  char ch;
  for (int i = 0; i < filesize; i++) {
    fseek(readfile, -1, SEEK_CUR);
    ch = fgetc(readfile);
    fseek(readfile, -1, SEEK_CUR);

    fwrite(&ch, sizeof(char), 1, writefile);
  }

  fclose(readfile);
  fclose(writefile);

  return 0;
}