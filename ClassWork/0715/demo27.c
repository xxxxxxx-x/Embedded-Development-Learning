/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0715/demo27.c
 * @brief        : 小练习：编写一个程序修改数值的大小端存储序。 例如:0x123456
 *                 修改后  0x563412
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-17 13:43:25
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    unsigned int number = 0, reversedNumber = 0;

    // 提示用户输入一个正整数
    printf("请输入一个正整数：");
    scanf("%u", &number); // 例如输入: 1193046 (0x123456)

    // 判断当前机器是大端还是小端
    unsigned short int testValue = 0x1234;
    _Bool isBigEndian = (*(char*)&testValue == 0x12);

    // 进行大小端转换
    reversedNumber = ((number & 0xFF000000) >> 24) |
                     ((number & 0x00FF0000) >> 8)  |
                     ((number & 0x0000FF00) << 8)  |
                     ((number & 0x000000FF) << 24);

    // 打印结果
    printf("输入的数值为：%#010x\n", number);
    printf("当前机器是%s\n", isBigEndian ? "大端" : "小端");
    printf("转换后的数值为：%#010x\n", reversedNumber);

    return 0;
}
