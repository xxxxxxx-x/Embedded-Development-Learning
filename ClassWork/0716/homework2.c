/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/homework2.c
 * @brief        : 作业：翻转一个数组中的元素     1,2,3,4,5  ->  5,4,3,2,1
 *                 演示了如何翻转一个整型数组的元素顺序，并输出结果。
 * @date         : 2024-07-16 19:53:54
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    // 定义并初始化一个包含5个元素的整型数组
    int array[5] = {1, 2, 3, 4, 5};

    unsigned int arrayLength = sizeof(array) / sizeof(int); // 计算数组长度

    // 使用循环翻转数组元素
    for (int i = 0, j = arrayLength - 1; i < arrayLength / 2; i++, j--) {
        int temp = array[i];   // 临时变量存储要交换的元素
        array[i] = array[j];   // 将末尾元素赋值给起始元素
        array[j] = temp;       // 将起始元素赋值给末尾元素
    }

    // 输出翻转后的数组元素
    printf("翻转后的数组元素为：");
    for (int i = 0; i < arrayLength; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
