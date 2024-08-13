/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0812/demo5/main.c
 * @brief        :
 *                 小练习：编写一个代码从键盘获取数据写入到文本中遇到#结束写入，结束后读取文件的所有内容并输出到屏幕终端显示
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 16:34:46
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
int main(int argc, const char *argv[]) {
  FILE *fp = fopen("my.txt", "w+");
  printf("请输入一些内容：");
  char ch;
  while ((ch = getchar()) != '#') {
    fwrite(&ch, 1, 1, fp);
  }
  fwrite("\0", 1, 1, fp);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);

  fseek(fp, 0, SEEK_SET);
  printf("写入的内容为：\n");
  char str[size];
  fread(str, size, 1, fp);
  printf("%s", str);
  return 0;
}