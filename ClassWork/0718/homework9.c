/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0718/homework9.c
 * @brief        :
 *                 作业：编写一个程序，去掉给定字符串中重复的字符。例如给定”google”，输出”gole”。（华为笔试题）
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-19 09:42:25
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  // 定义并初始化字符串
  char inputStr[1024] = "google";
  // 定义两个指针，一个用于遍历原字符串，一个用于构建新字符串
  char *srcPtr = inputStr;
  char *destPtr = inputStr;
  // 定义一个布尔数组，用于检查字符是否已经出现过
  bool charExist[256] = {false};

  // 遍历字符串
  while (*srcPtr) {
    // 如果字符未出现过，则将其添加到新字符串中，并标记为已出现
    if (!charExist[(unsigned char)*srcPtr]) {
      charExist[(unsigned char)*srcPtr] = true;
      *destPtr++ = *srcPtr;
    }
    srcPtr++;
  }

  // 在新字符串的末尾添加终止符
  *destPtr = '\0';

  // 打印结果
  printf("去重后的字符串: %s\n", inputStr);

  return 0;
}
