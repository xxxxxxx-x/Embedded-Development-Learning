/*******************************************************************************************************
 * @file         : demo25.c
 * @brief        : 小练习：求s=a+aa+aaa+aaaa+aa...a的值，其中a是一个数字。
 *                 例如2+22+222+2222+22222(此时共有5个数相加)，几个数相加有键盘控制
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-17 10:30:57
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *******************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    unsigned int totalSum = 0; // 总和
    unsigned int baseDigit; // 输入的基数数字
    unsigned int termCount; // 要相加的项数

    // 提示用户输入基数数字
    printf("请输入一个数字(1~9)：");
    scanf("%u", &baseDigit);

    // 提示用户输入要相加的项数
    printf("请输入几个数相加：");
    scanf("%u", &termCount);

    unsigned int currentTerm = 0; // 当前项的值
    for (unsigned int i = 1; i <= termCount; i++) {
        currentTerm = currentTerm * 10 + baseDigit; // 生成当前项，例如baseDigit=2时，currentTerm依次为2, 22, 222, ...
        totalSum += currentTerm; // 将当前项加到总和中
    }

    // 打印结果
    printf("结果是：%u\n", totalSum);

    return 0;
}
