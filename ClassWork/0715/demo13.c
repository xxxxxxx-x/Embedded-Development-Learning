/**************************************************************************************
 * @file         : demo13.c
 * @brief        : 小练习：输入一个正整数，输出它的阶乘。
 *                 注意：此代码假设输入的是正整数，不处理负数和零的情况。
 *                 阶乘是指将一个正整数及其前面所有正整数相乘得到的结果。
 *                 例如，输入5，则输出5的阶乘为5*4*3*2*1=120。
 *                 如果输入的是一个很大的正整数，确保代码能正确计算其阶乘。
 *                 此代码假设输入的数是有效的正整数。
 *                 阶乘的计算通过循环和数学运算实现。
 *                 输出结果包括原始输入的数和其阶乘的乘积形式。
 * @date         : 2024-07-15 19:14:19
 * @version      : 1.0
 * @note         :
 *                 这段代码从用户获取一个正整数，并计算输出其阶乘。
 *                 通过循环和数学运算实现阶乘的计算过程。
 **************************************************************************************/

#include <stdio.h>

unsigned long long factorial(int n); // 声明阶乘函数

int main(int argc, const char *argv[]) {
    unsigned int number;

    printf("请输入一个正整数：");
    scanf("%u", &number);

    printf("%u的阶乘是：%llu\n", number, factorial(number));

    return 0;
}

// 计算阶乘的函数，使用循环实现
unsigned long long factorial(int n) {
    unsigned long long result = 1;

    for (int i = 1; i <= n; ++i) {
        result *= i;
    }

    return result;
}
