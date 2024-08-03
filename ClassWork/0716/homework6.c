/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/homework6.c
 * @brief        : 作业：根据奇偶拆分一个数组为两个数组，并输出结果
 *                 将一个整型数组根据元素奇偶性分成两个数组，并打印输出两个新数组。
 * @date         : 2024-07-16 21:00:11
 * @version      : 1.0
 * @note         : 这个程序演示了如何根据条件将一个数组拆分成两个数组，并输出每个数组的内容。
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    // 定义原始数组和两个用于存储奇数和偶数的数组
    signed int originalArray[] = {
        3,    141,  59,   -65,  -589, -793, -238, -462, -643, -383, -279, -502,
        -884, -197, -169, -399, -375, -105, -820, -974, -944, 592,  307,  816,
        406,  286,  208,  998,  628,  34,   825,  342,  117,  67,   982,  148,
        1086, 513,  282,  306,  647,  1093, 844,  609,  550,  582,  231,  725,
        359,  408,  128,  481,  117,  450,  284,  102,  701,  938,  521,  105,
        559,  644,  622,  948,  954,  930,  381,  96};
    signed int oddArray[sizeof(originalArray) / sizeof(int)] = {0};   // 存储奇数的数组
    signed int evenArray[sizeof(originalArray) / sizeof(int)] = {0};  // 存储偶数的数组

    int oddIndex = 0, evenIndex = 0;

    // 根据元素奇偶性将元素分别存入对应的数组
    for (int i = 0; i < sizeof(originalArray) / sizeof(int); i++) {
        if (originalArray[i] % 2 != 0) {
            oddArray[oddIndex++] = originalArray[i];
        } else {
            evenArray[evenIndex++] = originalArray[i];
        }
    }

    // 输出奇数数组
    printf("奇数数组：\n");
    for (int i = 0; i < oddIndex; i++) {
        printf("%d ", oddArray[i]);
    }
    printf("\n");

    // 输出偶数数组
    printf("偶数数组：\n");
    for (int i = 0; i < evenIndex; i++) {
        printf("%d ", evenArray[i]);
    }
    printf("\n");

    return 0;
}
