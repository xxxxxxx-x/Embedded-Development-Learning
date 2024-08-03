/**************************************************************************************
 * @file         : demo17.c
 * @brief        : 输入一个正整数，判断它是否为回文数。例如：12321是回文数，12345不是回文数。
 *                 回文数是指从左到右和从右到左读都相同的数。例如：12321。
 *                 注意：此代码假设输入的是正整数，不处理负数和零的情况。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-15 20:25:43
 * @version      : 1.0
 * @note         :
 *                 这段代码从用户获取一个正整数，并判断其是否为回文数。
 *                 使用循环和数学运算判断数字的首位和末位是否相同，以确定是否为回文数。
 * @Copyright    :
 *                 (c)   2024-2025   a_23456@foxmail.com
 *                 All Right Reserved
 **************************************************************************************/

#include <stdbool.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    int num;
    printf("请输入一个正整数：");
    scanf("%d", &num);

    int original = num;
    int reversed = 0;

    // 反转数字
    while (num > 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }

    // 判断是否为回文数
    if (original == reversed) {
        printf("是回文数\n");
    } else {
        printf("不是回文数\n");
    }

    return 0;
}
