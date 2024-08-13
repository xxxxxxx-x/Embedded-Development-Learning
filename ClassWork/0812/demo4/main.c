/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0812/demo4/main.c
 * @brief        : 演示文件写入和缓冲区大小（避免访问内部实现）
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-12 15:54:43
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
    // 打开文件进行写操作
    FILE *fp = fopen("my.txt", "w");
    if (fp == NULL) {
        perror("打开文件失败");
        return -1;
    }

    // 写入数据
    const char *data = "helloworld!";
    size_t dataSize = fwrite(data, sizeof(char), 12, fp);

    // 检查写入是否成功
    if (dataSize != 12) {
        perror("写入文件失败");
        fclose(fp);
        return -1;
    }

    // 关闭文件
    fclose(fp);

    // 输出实际写入的数据量
    printf("实际写入的数据量是：%zu 字节\n", dataSize);

    return 0;
}
