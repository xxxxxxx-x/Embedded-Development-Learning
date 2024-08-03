/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0711/homework2.c
 * @brief        :
                    小练习：请编写程序接收这3个数据包，并解析出相应的数据[温度、湿度、门0-3、灯0-3]（门和灯的状态为0-关闭，1-打开）
                    现有嵌入式设备终端发送的数据包为32位无符号整数，该数据包中各位的含义如下：
•                   位00-07：表示温度数据
•                   位08-15：表示湿度数据
•                   位16-19：表示4个门的状态（门号0-3）
•                   位20-23：表示4个灯的状态（灯号0-3）
•                   24-31位：保留
                    以下是近期收到的数据包：0x12344520,0xff004B1c,0x00553C1E
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-11 16:35:31
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
*****************************************************************************************************************************************************************/

#include <stdio.h>

// 函数声明，将整数转换为二进制字符串
void int_to_binary_string(int num, char *binary_string, int size);

int packet_decapsulation() {
  // 数据包数组
  unsigned int data_packets[] = {0x12344520, 0xff004B1c, 0x00553C1E};
  int packet_count = sizeof(data_packets) / sizeof(data_packets[0]);

  // 解析每个数据包
  for (int i = 0; i < packet_count; ++i) {
    unsigned int data = data_packets[i];
    unsigned int temperature = data & 0xff;       // 温度
    unsigned int humidity = (data & 0xff00) >> 8; // 湿度
    unsigned int door = (data & 0xf0000) >> 16;   // 四门
    unsigned int light = (data & 0xf00000) >> 20; // 四灯

    // 定义存放二进制字符串的数组
    char s[9]; // 8位二进制数 + '\0'

    printf("数据包 %d:\n", i + 1);

    int_to_binary_string(temperature, s, 8);
    printf("温度的二进制是\t==>\t%s\n", s);
    int_to_binary_string(humidity, s, 8);
    printf("湿度的二进制是\t==>\t%s\n", s);
    int_to_binary_string(door, s, 8);
    printf("四门状态的二进制是\t==>\t%s\n", s);
    int_to_binary_string(light, s, 8);
    printf("四灯状态的二进制是\t==>\t%s\n", s);

    printf("温度是\t==>\t%d\n", temperature);
    printf("湿度是\t==>\t%d\n", humidity);

    printf("四个门分别的状态是\t==>\t%s\t==>\t%s\t==>\t%s\t==>\t%s\n",
           (door & 0x08) ? "开" : "关", (door & 0x04) ? "开" : "关",
           (door & 0x02) ? "开" : "关", (door & 0x01) ? "开" : "关");

    printf("四个灯分别的状态是\t==>\t%s\t==>\t%s\t==>\t%s\t==>\t%s\n",
           (light & 0x08) ? "开" : "关", (light & 0x04) ? "开" : "关",
           (light & 0x02) ? "开" : "关", (light & 0x01) ? "开" : "关");

    printf("\n");
  }

  return 0;
}

// 将整数转换为指定位数的二进制字符串
void int_to_binary_string(int num, char *binary_string, int size) {
  int i;
  for (i = size - 1; i >= 0; --i) {
    binary_string[i] = (num & 1) ? '1' : '0';
    num >>= 1;
  }
  binary_string[size] = '\0'; // 添加字符串结束符
}
