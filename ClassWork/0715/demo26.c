/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0715/demo26.c
 * @brief        : 
 *                 小练习：一球从100米高度自由落下，每次落地后反跳回原高度的一半；再落下，求它在第10次落地时，共经过多少米？第10次反弹多高？
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-17 11:26:16
 * @version      : 1.0
 * @note         : 
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    double initialHeight = 100.0; // 初始高度
    double totalDistance = initialHeight; // 总距离，包括第一次下落的高度
    double reboundHeight = initialHeight / 2; // 第一次反弹的高度

    // 计算总距离和第10次反弹的高度
    for (int i = 2; i <= 10; i++) {
        totalDistance += 2 * reboundHeight; // 每次落地后的反弹和下落
        reboundHeight /= 2; // 反弹高度减半
    }

    printf("第10次落地时经过的总距离是：%lf米\n", totalDistance);
    printf("第10次反弹的高度是：%lf米\n", reboundHeight);

    return 0;
}
