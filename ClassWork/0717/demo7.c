/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0717/demo7.c
 * @brief        : 小练习：利用指针将 value 的值修改为大端存储格式
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-17 16:18:48
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/
 
#include <stdio.h>

int main() {
    unsigned int value = 0xabcd1234;  // 初始化一个32位整数 value

    unsigned char *p = (unsigned char *)&value;  // 定义指向 value 的字节型指针

    // 使用指针交换字节顺序，实现大端存储格式
    for (int i = 0, j = 3; i < 2; i++, j--) {
        unsigned char tmp = *(p + j);
        *(p + j) = *(p + i);
        *(p + i) = tmp;
    }

    printf("修改后的 value 的大端存储格式为：%0#10x\n", value);  // 输出修改后的 value

    return 0;
}
