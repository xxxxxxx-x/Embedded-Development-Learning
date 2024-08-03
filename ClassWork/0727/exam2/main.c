/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0727/exam2/main.c
 * @brief        :
 *
                   试卷编程题：请编程实现一个函数：传入一个八位无符号整数，生成并返回此数的二进制数倒序字符串。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-29 10:31:32
 * @version      : 1.1
 * @note         : 这道题我太粗心了
                    1. 忘记了优先级的问题：*str = (num & (0x01 << i)) >> i;
                    2. 在字符赋值时应该加上48，即 '0' 的ASCII码值
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

// 定义函数，将八位无符号整数转换为二进制并倒序存储到字符串中
void reverseBinaryString(unsigned char num, char *str) {
  // 将最低位转换为字符存储到字符串中
  *str = (num & 0x01) + '0';
  str++;
  // 依次处理剩下的7位
  for (int i = 1; i <= 7; i++, str++) {
    *str = ((num & (0x01 << i)) >> i) + '0';
  }
  *str = '\0'; // 添加字符串结束符
}

int main(int argc, const char *argv[]) {
  unsigned char num;
  char binaryStr[9] = {0}; // 二进制字符串长度为8，加上结束符共9个字符
  printf("请输入一个八位无符号整数："); // 提示用户输入一个八位无符号整数
  scanf("%hhu", &num);
  reverseBinaryString(num, binaryStr); // 调用函数生成二进制倒序字符串
  printf("二进制倒序字符串为：%s\n", binaryStr); // 输出结果
  return 0;
}

//  下面是原始代码注释掉的部分供参考

// void func(int num, char *str) {
//     *str = num & 0x01; //
//     错误1：忘记了操作符优先级，应该加上括号并加上48，即'0'的ASCII码值：*str =
//     (num & 0x01) + 48; str++; for (int i = 1; i <= 7; i++, str++) {
//         *str = (num & (0x01 << i)) >> i; //
//         错误2：同样需要加上括号并加上48：*str = ((num & (0x01 << i)) >> i) +
//         48;
//     }
//     // 注意：没有添加字符串结束符，应在循环后添加：*str = '\0';
// }

// int main(int argc, const char *argv[]) {
//     unsigned int num; // 错误3：应使用unsigned
//     char类型以确保处理八位无符号整数 char str[10]; //
//     注意：这里声明了长度为10的字符数组，但实际使用时只需要长度为9（8位二进制数加结束符）
//     printf("请输入一个八位无符号整数："); //
//     注意：提示信息末尾需要添加冒号和空格 scanf("%u", &num); //
//     错误4：应使用%hhu格式以确保读取八位无符号整数 func(num, str);
//     printf("%s\n", str); // 注意：输出结果前需要确保str以'\0'结束
//     return 0;
// }
