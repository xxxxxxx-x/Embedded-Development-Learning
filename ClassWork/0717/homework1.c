/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0717/homework1.c
 * @brief        : 作业：利用指针删除字符串中的空格
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-17 17:23:36
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>

int main() {
  char input_string[] = "hello   wo rld"; // 初始化输入字符串数组
  char result_string[20];         // 用于存储删除空格后的字符串
  char *input_ptr = input_string; // 指向输入字符串数组的指针

  int result_index = 0;
  for (int i = 0; input_string[i] != '\0'; i++) { // 遍历输入字符串
    if (*(input_ptr + i) != ' ') { // 如果当前字符不是空格
      result_string[result_index++] =
          *(input_ptr + i); // 将非空格字符存入结果字符串
    }
  }
  result_string[result_index] = '\0'; // 结尾添加字符串结束符

  printf("删除空格后的字符串为：%s\n", result_string); // 输出删除空格后的字符串

  return 0;
}
