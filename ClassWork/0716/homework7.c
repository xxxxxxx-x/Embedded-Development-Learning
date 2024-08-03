/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/homework7.c
 * @brief        : 作业：删除一个数组中重复的元素，并输出结果
 *                 给定一个整型数组，删除其中重复的元素，并输出不重复的元素序列。
 * @date         : 2024-07-16 21:20:03
 * @version      : 1.0
 * @note         : 这个程序演示了如何从一个数组中删除重复的元素，并输出剩余的不重复元素序列。
  * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
    signed int originalArray[] = {1, 2, 3, 2, 3, 2};
    signed int uniqueArray[10] = {0};  // 用于存储不重复的元素
    unsigned int uniqueCount = 0;      // 不重复元素的个数

    // 遍历原始数组
    for (int i = 0; i < 6; i++) {
        _Bool isDuplicate = false;

        // 检查当前元素是否已存在于uniqueArray中
        for (int j = 0; j < uniqueCount; j++) {
            if (originalArray[i] == uniqueArray[j]) {
                isDuplicate = true;
                break;
            }
        }

        // 如果不是重复元素，则添加到uniqueArray中
        if (!isDuplicate) {
            uniqueArray[uniqueCount++] = originalArray[i];
        }
    }

    // 输出不重复元素序列
    printf("删除重复元素后的数组：\n");
    for (int i = 0; i < uniqueCount; i++) {
        printf("%d ", uniqueArray[i]);
    }
    printf("\n");

    return 0;
}
