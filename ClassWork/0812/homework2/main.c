/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0812/homework2/main.c
 * @brief        : 作业：利用数据块操作，往文件写入一个结构体，并读取出来.
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 19:04:37
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义学生结构体
struct student {
  char name[50]; // 学生的姓名
  int age;       // 学生的年龄
  int id;        // 学生的学号
};

int main(int argc, const char *argv[]) {
  struct student s1;

  strncpy(s1.name, "张三", sizeof(s1.name));
  s1.age = 18;
  s1.id = 12345;

  printf("存入学生信息：\n");
  printf("姓名: %s\n", s1.name);
  printf("年龄: %d\n", s1.age);
  printf("学号: %d\n", s1.id);

  FILE *file = fopen("my.txt", "w+");

  if (file == NULL) {
    perror("无法打开文件");
    exit(EXIT_FAILURE);
  }

  fwrite(&s1, sizeof(struct student), 1, file);

  fseek(file, 0, SEEK_SET);

  struct student s2;

  fread(&s2, sizeof(struct student), 1, file);

  printf("读取学生信息：\n");
  printf("姓名: %s\n", s2.name);
  printf("年龄: %d\n", s2.age);
  printf("学号: %d\n", s2.id);

  fclose(file);

  return 0;
}