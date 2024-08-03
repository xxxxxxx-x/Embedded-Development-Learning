/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0711/homework1.c
 * @brief        :
                    作业1：编写一个程序实现数据包的封装，根据用户输入的温度，湿度，门的状态0/1
                    灯的状态0/1  空调状态0/1 冰箱状态0/1
                    当前二氧化碳浓度,封装一个32位的数据包.
                    温度:0-6位，湿度7-13位，14位表示门，15位表示灯，16位表示空调，17位表示冰箱，18-25位表示二氧化碳浓度。
                    并输出该数据包！
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-11 18:47:36
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
*****************************************************************************************************************************************************************/

#include <stdio.h>

int packet_encapsulation() {
  unsigned short int temperature;       // 温度（0-6位）
  unsigned short int humidity;          // 湿度（7-13位）
  unsigned char door;                   // 门状态 (0 或 1)
  unsigned char light;                  // 灯状态 (0 或 1)
  unsigned char ac;                     // 空调状态 (0 或 1)
  unsigned char fridge;                 // 冰箱状态 (0 或 1)
  unsigned short int co2_concentration; // 二氧化碳浓度（18-25位）
  unsigned int data;                    // 32位的数据包

  // 输入数据
  printf("请输入温度（0~127）：");
  scanf("%hu", &temperature);
  printf("请输入湿度（0~127）：");
  scanf("%hu", &humidity);
  printf("请输入门的状态（0表示关，1表示开）：");
  scanf("%hhu", &door);
  printf("请输入灯的状态（0表示关，1表示开）：");
  scanf("%hhu", &light);
  printf("请输入空调状态（0表示关，1表示开）：");
  scanf("%hhu", &ac);
  printf("请输入冰箱状态（0表示关，1表示开）：");
  scanf("%hhu", &fridge);
  printf("请输入二氧化碳浓度（0~255）：");
  scanf("%hu", &co2_concentration);

  // 封装数据包
  data = temperature | (humidity << 7) | (door << 14) | (light << 15) |
         (ac << 16) | (fridge << 17) | (co2_concentration << 18);

  // 输出封装后的数据包
  printf("封装完成的数据包为：\t==>\t%0#10X\n", data);

  return 0;
}
