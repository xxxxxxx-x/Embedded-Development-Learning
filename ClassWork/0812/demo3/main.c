/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0812/demo3/main.c
 * @brief        :
 *                 小练习：往一个文件中写入一个浮点数组，并从文件中读取一个浮点数组到内存中
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 14:48:19
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, char *argv[]) {
  // 定义浮点数组
  float writeBuffer[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
  float readBuffer[5] = {0.0}; // 用于存储从文件中读取的数据

  // 打开文件进行写操作
  FILE *fwrite1 = fopen("my.txt", "w");
  if (fwrite1 == NULL) {
    perror("打开文件失败进行写操作");
    return -1;
  }

  // 将浮点数组写入文件
  if (fwrite(writeBuffer, sizeof(float), 5, fwrite1) != 5) {
    perror("写入文件失败");
    fclose(fwrite1);
    return -1;
  }
  fclose(fwrite1);

  // 打开文件进行读操作
  FILE *fread1 = fopen("my.txt", "r");
  if (fread1 == NULL) {
    perror("打开文件失败进行读操作");
    return -1;
  }

  // 从文件中读取浮点数组
  if (fread(readBuffer, sizeof(float), 5, fread1) != 5) {
    perror("读取文件失败");
    fclose(fread1);
    return -1;
  }
  fclose(fread1);

  // 输出读取的浮点数组
  printf("从文件中读取的浮点数组：\n");
  for (int i = 0; i < 5; i++) {
    printf("%f ", readBuffer[i]);
  }
  printf("\n");

  return 0;
}
