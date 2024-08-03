/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0723/demo5/main.c
 * @brief        :
 *小练习：利用字符串的操作接口，实现提前指定字符串"www.baidu.com"中的"com"提取出来
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-23 16:31:31
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  // 定义一个字符串，包含我们需要操作的内容
  const char *fullStr = "www.baidu.com";

  // 使用strstr函数找到子串"com"的位置
  char *substr = strstr(fullStr, "com");

  // 判断是否找到了子串
  if (substr == NULL) {
    // 如果没有找到，输出提示信息
    printf("找不到子串\"com\"\n");
  } else {
    // 如果找到了，输出子串及其后的内容
    printf("提取的子串为：%s\n", substr);
  }

  return 0;
}
