/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0723/homework1/main.c
 * @brief        : 测试自定义字符串处理函数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-23 19:17:08
 * @version      : 1.0
 * @note         : 该文件测试了自定义的字符串处理函数
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Rights Reserved
 *****************************************************************************************************************************************************************/

#include "myString.h"
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  // 测试 myStrlen 函数
  const char *testStr = "Hello, World!";
  printf("测试 myStrlen:\n");
  printf("字符串 \"%s\" 的长度是 %d\n", testStr, myStrlen(testStr));
  printf("\n");

  // 测试 myStrcpy 函数
  char dest[50];
  myStrcpy(dest, testStr);
  printf("测试 myStrcpy:\n");
  printf("源字符串: \"%s\"\n", testStr);
  printf("复制后的字符串: \"%s\"\n", dest);
  printf("\n");

  // 测试 myStrncpy 函数
  char dest2[50];
  myStrncpy(dest2, testStr, 5); // 复制前 5 个字符
  dest2[5] = '\0';              // 添加字符串结束符
  printf("测试 myStrncpy:\n");
  printf("源字符串: \"%s\"\n", testStr);
  printf("复制前 5 个字符: \"%s\"\n", dest2);
  printf("\n");

  // 测试 myBzero 函数
  char buffer[10] = "ABCDEFGH";
  myBzero(buffer, 5); // 将前 5 个字节置零
  printf("测试 myBzero:\n");
  printf("置零后的字符串: \"%s\"\n", buffer);
  printf("\n");

  // 测试 myStrcat 函数
  char dest3[50] = "Hello";
  myStrcat(dest3, " World!");
  printf("测试 myStrcat:\n");
  printf("连接后的字符串: \"%s\"\n", dest3);
  printf("\n");

  // 测试 myStrchr 函数
  char *result = myStrchr(testStr, 'W');
  printf("测试 myStrchr:\n");
  if (result) {
    printf("字符 'W' 在字符串 \"%s\" 中的位置: %ld\n", testStr,
           result - testStr);
  } else {
    printf("字符 'W' 未找到\n");
  }
  printf("\n");

  // 测试 myStrcmp 函数
  int cmpResult = myStrcmp("a", "abc");
  printf("测试 myStrcmp:\n");
  printf("\"a\" 与 \"abc\" 比较结果: %d\n", cmpResult);
  printf("\n");

  // 测试 myStrncasecmp 函数
  cmpResult = myStrncasecmp("a", "ABC", 3);
  printf("测试 myStrncasecmp:\n");
  printf("\"a\" 与 \"ABC\" 前 3 个字符比较结果: %d\n", cmpResult);
  printf("\n");

  // 测试 myStrstr 函数
  char *found = myStrstr(testStr, "World");
  printf("测试 myStrstr:\n");
  if (found) {
    printf("子字符串 \"World\" 在字符串 \"%s\" 中的位置: %ld\n", testStr,
           found - testStr);
  } else {
    printf("子字符串 \"World\" 未找到\n");
  }
  printf("\n");

  // 测试 myStrtok 函数
  char str[] = "one,two,three";
  char *token = myStrtok(str, ",");
  printf("测试 myStrtok:\n");
  while (token) {
    printf("分割出的字段: \"%s\"\n", token);
    token = myStrtok(NULL, ",");
  }
  printf("\n");

  // 测试 myAtoi 函数
  printf("测试 myAtoi:\n");
  printf("字符串 \"9875112\" 转换为整数: %d\n", myAtoi("9875112"));

  return 0;
}
