/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0717/homework2.c
 * @brief        : 作业：利用指针翻转字符串中的单词
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-17 18:30:43
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>
#include <string.h>

// 函数声明：翻转单个单词
void reverseWord(char *start, char *end) {
  char tmp;
  while (start < end) {
    tmp = *start;
    *start++ = *end;
    *end-- = tmp;
  }
}

int main() {
  char str[] = "hello 12345";
  char *p = str;
  char *start = str;

  // 寻找单词边界并翻转单词
  while (*p) {
    // 找到单词的起始位置
    while (*p && *p != ' ') {
      p++;
    }
    // p 指向单词结束的下一个位置，start 指向单词的起始位置
    reverseWord(start, p - 1);

    // 跳过空格找下一个单词的起始位置
    while (*p && *p == ' ') {
      p++;
    }
    start = p; // 更新下一个单词的起始位置
  }

  printf("翻转后的字符串为：%s\n", str);

  return 0;
}
