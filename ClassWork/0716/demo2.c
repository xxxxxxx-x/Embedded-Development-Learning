/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/demo2.c
 * @brief        : 小练习：定义一个float数组同时初始化，并通过下标和解引用访问数组元素
 *                 演示了如何定义和初始化一个float数组，并通过下标和指针解引用访问数组元素的方法。
 * @date         : 2024-07-16 14:47:53
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    float array[5] = {10.0, 20.0, 30.0, 40.0, 50.0}; // 定义并初始化一个包含5个float元素的数组

    // 使用数组下标访问数组元素
    printf("array[0] = %.2f\n", array[0]);
    printf("array[1] = %.2f\n", array[1]);
    printf("array[2] = %.2f\n", array[2]);
    printf("array[3] = %.2f\n", array[3]);
    printf("array[4] = %.2f\n", array[4]);

    // 使用指针解引用访问数组元素
    printf("*(array + 0) = %.2f\n", *(array + 0));
    printf("*(array + 1) = %.2f\n", *(array + 1));
    printf("*(array + 2) = %.2f\n", *(array + 2));
    printf("*(array + 3) = %.2f\n", *(array + 3));
    printf("*(array + 4) = %.2f\n", *(array + 4));

    return 0;
}
