/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0716/homework3.c
 * @brief        : 作业：求一个数组中的最大值与最小值
 *                 演示了如何找出整型数组中的最大值和最小值，并输出结果。
 * @date         : 2024-07-16 20:06:07
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    // 定义并初始化整型数组
    int array[] = {3, 141, 59, -65, -589, -793, -238, -462, -643, -383, -279, -502,
                   -884, -197, -169, -399, -375, -105, -820, -974, -944, 592, 307,
                   816, 406, 286, 208, 998, 628, 34, 825, 342, 117, 67, 982, 148,
                   1086, 513, 282, 306, 647, 1093, 844, 609, 550, 582, 231, 725,
                   359, 408, 128, 481, 117, 450, 284, 102, 701, 938, 521, 105,
                   559, 644, 622, 948, 954, 930, 381, 96};

    // 初始化最大值和最小值为数组第一个元素
    int max = array[0];
    int min = array[0];

    // 遍历数组，更新最大值和最小值
    for (int i = 1; i < sizeof(array) / sizeof(int); i++) {
        if (array[i] > max) {
            max = array[i];
        }

        if (array[i] < min) {
            min = array[i];
        }
    }

    // 输出最大值和最小值
    printf("最大值：%d\t最小值：%d\n", max, min);

    return 0;
}
