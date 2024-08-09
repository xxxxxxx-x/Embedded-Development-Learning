/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/demo1/main.c
 * @brief        : 小练习：利用main函数的传参，实现简单的四则运算
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-09 10:21:33
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>  // 包含标准输入输出的头文件
#include <stdlib.h> // 包含标准库函数的头文件

int main(int argc, const char *argv[]) {
  // 检查参数个数是否正确，正确的格式应该是：程序名 运算值1 运算符 运算值2
  if (argc != 4) {
    fprintf(stderr, "用法: %s <运算值1> <运算符> <运算值2>\n", argv[0]);
    return 1; // 返回错误代码 1，表示参数不足
  }

  // 将参数转换为浮点数类型
  double num1 = atof(argv[1]);
  double num2 = atof(argv[3]);
  char oper = argv[2][0];

  // 打印运算过程，便于调试
  printf("操作: %s %c %s\n", argv[1], oper, argv[3]);

  // 根据运算符进行相应的运算
  switch (oper) {
  case '+': {
    printf("结果: %.2f\n", num1 + num2);
    break;
  }
  case '-': {
    printf("结果: %.2f\n", num1 - num2);
    break;
  }
  case '*': {
    printf("结果: %.2f\n", num1 * num2);
    break;
  }
  case '/': {
    // 检查除数是否为0
    if (num2 == 0) {
      fprintf(stderr, "错误: 被除数不能为0\n");
      return 1; // 返回错误代码 1，表示除数为0
    }
    printf("结果: %.2f\n", num1 / num2);
    break;
  }
  default: {
    // 如果运算符不是以上四种，报错
    fprintf(stderr, "错误: 无效的运算符 '%c'\n", oper);
    return 1; // 返回错误代码 1，表示运算符错误
  }
  }

  return 0; // 程序成功结束
}
