/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/homework4.c
 * @brief        : 判断一个数组中的元素是否为回文
 *                 演示了如何判断整型数组是否为回文，并输出判断结果。
 * @date         : 2024-07-16 20:20:38
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    // 定义整型数组并初始化
    int array[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
    unsigned int arrayLength = sizeof(array) / sizeof(int);
    bool isPalindrome = true;

    // 使用双指针法判断是否为回文
    for (int i = 0, j = arrayLength - 1; i < arrayLength / 2; i++, j--) {
        if (array[i] != array[j]) {
            isPalindrome = false;
            break;
        }
    }

    // 输出判断结果
    printf(isPalindrome ? "是回文数\n" : "不是回文数\n");

    return 0;
}
