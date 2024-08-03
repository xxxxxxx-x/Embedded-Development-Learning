/**************************************************************************************
 * @file         : demo12.c
 * @brief        : 小练习：判断一个年份是否为闰年。
 *                 闰年的判断公式：
 *                 1. 能被4整除并且不能被100整除
 *                 2. 能被400整除
 *                 闰年的判断通过逻辑运算符和条件语句实现。
 *                 如果输入的年份符合上述任一条件，则判定为闰年，否则判定为非闰年。
 * @date         : 2024-07-15 19:08:46
 * @version      : 1.0
 * @note         :
 *                 这段代码从用户获取一个年份，并判断输出是否为闰年。
 *                 通过逻辑运算符和条件语句实现闰年的判断。
 **************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    unsigned int year;

    printf("请输入一个年份：");
    scanf("%u", &year);

    // 判断是否为闰年
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        printf("%u年是闰年\n", year);
    } else {
        printf("%u年不是闰年\n", year);
    }

    return 0;
}
