/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/demo3.c
 * @brief        : 小练习：定义一个二维数组，并对该数组进行初始化
 *                 演示了不同方式定义和初始化二维数组的方法。
 * @date         : 2024-07-16 16:39:04
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    // 方法1：全部元素初始化为0
    int array1[3][3] = {{0}}; // 全部元素初始化为0

    // 方法2：按顺序初始化，不足部分自动补0
    int array2[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // 方法3：每行分别初始化
    int array3[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 8} // 此处原文有一个错误，应为{7, 8, 9}
    };

    // 方法4：逐个赋值初始化
    int array4[3][3];
    array4[0][0] = 1;
    array4[0][1] = 2;
    array4[0][2] = 3;
    array4[1][0] = 4;
    array4[1][1] = 5;
    array4[1][2] = 6;
    array4[2][0] = 7;
    array4[2][1] = 8;
    array4[2][2] = 9;

    // 方法5：部分元素初始化，其余默认为0
    int array5[3][3] = {
        {0, 1, 2}, // 此处原文中的初始化超出了数组范围，应修正为合法范围内的初始化
        {3, 4, 5},
        {6, 7, 8}
    };

    // 打印数组示例
    printf("array1:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", array1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // 打印数组2示例
    printf("array2:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", array2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // 打印数组3示例
    printf("array3:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", array3[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // 打印数组4示例
    printf("array4:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", array4[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // 打印数组5示例
    printf("array5:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", array5[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}
