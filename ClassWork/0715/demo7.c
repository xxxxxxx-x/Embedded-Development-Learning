/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0715/demo7.c
 * @brief        :
 *                 小练习：编写一个程序，该程序读取输入直到遇到#字符，然后报告读取的空格数目、读取的换行符数目以及读取
 *                 的所有其他字符数目
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-15 14:27:41
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

int main(int argc, const char *argv[]) {
  int Space = 0, LineBreak = 0, Other = 0;
  char ch;

  printf("请输入一些文字(以#结束)：");

  while (1) {
    // 从标准输入读取一个字符
    scanf("%c", &ch);

    // 根据读取的字符类型进行统计
    if (ch == '\n') {
      LineBreak++; // 统计换行符数目
    } else if (ch == ' ') {
      Space++; // 统计空格数目
    } else if (ch == '#') {
      break; // 遇到 '#' 字符时结束循环
    } else {
      Other++; // 统计其他字符数目
    }
  }

  // 输出统计结果
  printf("空格数为: %d, 换行符数为: %d, 其他字符数为: %d\n", Space, LineBreak,
         Other);

  return 0;
}
