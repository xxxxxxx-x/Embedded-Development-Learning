/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0727/exam3/main.c
 * @brief        :
 *                 设计函数 int myatoi(char *s)。
 *                 函数说明：myatoi()会扫描参数s字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，而再遇到非数字或字符串结束时('\0')才结束转换，并将结果返回。
 *                 返回值 返回转换后的整型数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-29 11:29:34
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0727/exam3/main.c
 * @brief        : 设计函数 int myatoi(char
 **s)。函数说明：myatoi()会扫描参数s字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，而再遇到非数字或字符串结束时('\0')才结束转换，并将结果返回。
 *                 返回值 返回转换后的整型数
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-29 11:29:34
 * @version      : 1.1
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 将字符串转换为整数
int myatoi(char *str) {
  char *start = str;
  char *end;
  int width = 1; // 设置数字的位数
  int result = 0;
  int sign;

  // 跳过前导空格
  while (*start == ' ') {
    start++;
  }

  // 获取正负号的位置
  if (*start == '+' || *start == '-') {
    sign = (*start == '+') ? 1 : -1;
    start++;
  } else if (*start >= '0' && *start <= '9') {
    sign = 1;
  } else {
    // 如果既没有正负号也没有数字，退出程序
    printf("没有找到有效的数字，程序退出！\n");
    exit(1);
  }

  // 从正负号后一位的位置开始到末尾，找到第一个非数字字符
  end = start;
  while (*end >= '0' && *end <= '9') {
    end++;
    width *= 10;
  }

  // 计算数字部分的值
  width /= 10;
  while (*start >= '0' && *start <= '9') {
    result += (*start - '0') * width;
    width /= 10;
    start++;
  }

  return result * sign;
}

int main() {
  printf("%d\n", myatoi("      +1234afa56dawfgease"));
  return 0;
}

// 下面是原始代码注释掉的部分供参考

// int myatoi(char *s){
//     char *start = s;
//     char *end;
//     int width = 1; // 设置数字的位数
//     int resule; // 错误1：变量名拼写错误，应该是result而不是resule

//     // 获取正负号的位置
//     while (*start != '\0') {
//         if (*start == '+' || *start == '-') {
//             break;
//         }
//         start++;
//     }

//     // 判断是否有正负号
//     if (*start == '\0') {
//         printf("没有正负号，程序退出！\n"); //
//         错误2：没有正负号并不意味着没有有效数字 exit(1);
//     }

//     // 从正负号后一位的位置开始到末尾
//     end = start + 1;
//     while (*end != '\0') {
//         if (!(*end >= '0' || *end <= '9')) { // 错误3：条件应为(*end < '0'
//         ||*end > '9')，否则会一直循环
//             break;
//         }
//         end++;
//         width *= 10;
//     }

//     width /= 10;
//     result = *start == '+' ? 1 : -1; // 判断正负
//     start = start + 1;
//     while (*start != *end) { // 错误4：条件应为(*start !=
//     '\0')，否则循环不会到达数字的末尾
//         result += (*start - 48) * width;
//         //错误5：应初始化result为0，然后在此基础上累加
//      width /= 10; start++;
//     }
//     return result;
// }
