/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0717/homework3.c
 * @brief        : 作业：反转每个单词中的字符
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-18 11:13:37
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
#include <stdio.h>
#include <string.h> // 引入strlen函数

int main(int argc, const char *argv[]) {
    char inputStr[1024];
    printf("请输入一些单词: ");
    fgets(inputStr, sizeof(inputStr), stdin); // 使用fgets替代gets更安全

    // 去除输入字符串末尾的换行符
    inputStr[strcspn(inputStr, "\n")] = '\0';

    char *start = inputStr;
    char *end = inputStr;

    while (*start != '\0') {
        // 找到单词的结尾
        while (*(end + 1) != ' ' && *(end + 1) != '\0') {
            end++;
        }

        // 计算单词长度
        int wordLen = end - start;

        // 反转单词
        for (int i = 0; i <= wordLen / 2; i++) {
            char tmp = start[i];
            start[i] = start[wordLen - i];
            start[wordLen - i] = tmp;
        }

        // 移动到下一个单词的开始
        start = end + 1;
        while (*start == ' ') {
            start++;
        }
        end = start;
    }

    printf("反转后的字符串: %s\n", inputStr);

    return 0;
}
