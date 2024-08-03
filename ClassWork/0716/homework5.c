/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/homework5.c
 * @brief        : 合并两个整型数组为一个新数组，并输出结果
 *                 演示了如何将两个整型数组合并为一个新数组，并打印输出合并后的结果。
 * @date         : 2024-07-16 20:29:02
 * @version      : 1.0
 * @note         : 这个程序通过静态数组实现了数组合并操作，适用于大多数编译器和平台。
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    // 定义两个整型数组并初始化
    int firstArray[10] = {3, 141, 59, -65, -589, -793, -238, -462, -643, -383};
    int secondArray[10] = {359, 408, 128, 481, 117, 450, 284, 102, 701, 938};
    int mergedArray[20]; // 新数组，大小为两个数组之和

    // 将firstArray和secondArray合并到mergedArray中
    int index = 0;
    for (; index < 10; index++) {
        mergedArray[index] = firstArray[index];
    }

    for (int j = 0; j < 10; j++, index++) {
        mergedArray[index] = secondArray[j];
    }

    // 输出合并后的数组
    printf("合并后的数组：\n");
    for (int k = 0; k < 20; k++) {
        printf("%d ", mergedArray[k]);
    }
    printf("\n");

    return 0;
}
