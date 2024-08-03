#ifndef _LCD_H
#define _LCD_H
/**
 * @file name : LCD.c
 * @brief     : LCD屏幕显示相关功能函数
 * @author    : RISE_AND_GRIND@163.com
 * @date      : 2024/05/17
 * @version   : 1.0
 * @note      :
 *              默认800*480分辨率
 *              编译命令 arm-linux-gcc main.c ./src/LCD.c -o ./demo.out -I ./include -L ./lib -ljpeg
 * CopyRight (c)  2023-2024   RISE_AND_GRIND@163.com   All Right Reseverd
 */
/***************************************头文件***************************************/
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "./font.h"
/***************************************END******************************************/
/***************************************宏定义***************************************/
/*-------------颜色显示-------------*/
#define getColor1(a, b, c, d) (a | b << 8 | c << 16 | d << 24)
/*-----------------END-------------*/
/*-------------设备参数-------------*/
#define LCD_PATH "/dev/fb0"                            // LCD屏的设备路径，根据实际情况修改
#define LCD_MAPSIZE (((LCD_Width) * (LCD_Height)) * 4) // LCD内存映射的大小 长*宽*色深
#define LCD_Width (800)                                // LCD宽 像素
#define LCD_Height (480)                               // LCD高 像素
#define FONT_PATH "../font/AlimamaShuHeiTi-Bold.ttf"
/*-----------------END-------------*/
/***************************************END******************************************/
/***************************************数据类型*************************************/
/*-------------无符号-------------*/
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
/*-----------------END-------------*/
/*-------------有符号-------------*/
typedef char s8_t;
typedef short s16_t;
typedef int s32_t;
typedef long long s64_t;
/*-----------------END-------------*/
/*-------------设备参数-------------*/
// 构造记录LCD硬件参数的数据类型
typedef struct lcd_deviceinfo
{
    int fd;  // 用于记录LCD的文件描述符
    int *mp; // 用于记录LCD的内存映射地址
} LcdDev;
/*-----------------END-------------*/

/***************************************END******************************************/

/**
 * @name      lcd_init
 * @brief     LCD屏的初始化, 打开驱动文件、内存映射
 * @param     lcdpath 指的是LCD屏的设备路径
 * @return
 *      @retval    初始化成功，则返回LCD屏的硬件信息的堆内存地址，失败则返回NULL
 * @date      2024/05/17
 * @version   1.0
 * @note      在程序结束之前，需要手动释放申请的堆内存地址
 *            LcdDev *lcd=lcd_init(LCD_PATH)
 */
LcdDev *lcd_init(const char *lcdpath);

/**
 * @name      lcd_uninit
 * @brief     LCD屏的解除初始化 关闭文件 释放内存映射
 * @param     plcd 记录LCD硬件信息的堆内存地址
 * @return
 *      @retval    成功，则返回true，失败，则返回false
 * @date      2024/05/17
 * @version   1.0
 * @note      ● 在程序结束之前，需要调用该函数对申请的LCD相关资源进行手动释放。
 *            ● 调用该函数之前，必须提前调用过lcd_init()，否则程序会段错误！！！
 */
bool lcd_uninit(LcdDev *plcd);

/**
 * @name      Show_Font_Lcd
 * @brief     将一个GB2312汉字显示在LCD任意位置 无画布
 * @param     plcd LCD硬件参数结构体(文件描述符 内存映射地址)
 * @param     HZKPath 取模生成的汉字库路径  例如"./font/HZK16.dzk"
 * @param     HZKsize 汉字库每个汉字的宽度(高度) 单位像素点, 例如16*16
 * @param     font_buf  要显示的一个汉字  "我" 只能是GB2312编码
 * @param     x 显示位置的x坐标 左上角开始
 * @param     y 显示位置的y坐标
 * @param     color 要显示的颜色 0xFFFFFFFF
 * @return
 *      @retval    true 显示成功
 *      @retval    false 显示失败
 * @date      2024年5月17日20:30:43
 * @version   V1.0 2024年5月17日20:30:43 测试随机位置成功
 * @note
 * 前提:        LcdDev *lcd= lcd_init(LCD_PATH); // LCD的初始化
 * 使用 :
 *              Show_Font_Lcd(lcd, "./font/HZK16.dzk", 16, "我", 400, 300, 0xFF0000FF);
 ****************************
 *   16*16的某个汉字的点阵
 *       0         1
 *   0111 0000 1100 0000  0
 *   0111 0000 1100 0000  1
 *   0111 0000 1100 0000  2
 *   0111 0000 1100 0000  3
 *   0111 0000 1100 0000  4
 *   0111 0000 1100 0000
 *
 ****************************/
bool Show_Font_Lcd(LcdDev *plcd, char *HZKPath, int HZKsize, unsigned char font_buf[], int x, int y, unsigned int color);
/**
 * @name      Show_zhString_Lcd
 * @brief     将中文字符串 GB2312汉字显示在LCD任意位置 无画布
 * @param     plcd LCD硬件参数结构体(文件描述符 内存映射地址)
 * @param     HZKPath 取模生成的汉字库路径  例如"./font/HZK16.dzk"
 * @param     HZKsize 汉字库每个汉字的宽度(高度) 单位像素点, 例如16*16
 * @param     zhString_buf  要显示的字符串汉字  "我" 只能是GB2312编码
 * @param     x 显示位置的x坐标 左上角开始
 * @param     y 显示位置的y坐标
 * @param     color 要显示的颜色 0xFFFFFFFF
 * @return
 *      @retval    true 显示成功
 *      @retval    false 显示失败
 * @date
 * @version   V1.0 2024年5月17日21:09:18 测试后, 可以显示字符串
 * @note
 * 前提:        LcdDev *lcd= lcd_init(LCD_PATH); // LCD的初始化
 * 使用 :
 *              unsigned char zhString[] = "我爱中国！！";
 *              Show_zhString_Lcd(lcd, "./font/HZK16.dzk", 16, zhString, 100, 100, 0xFF0000FF);
 */
bool Show_zhString_Lcd(LcdDev *plcd, char *HZKPath, int HZKsize, unsigned char zhString_buf[], int x, int y, unsigned int color);

/**
 * @name      displayText
 * @brief     显示字符串函数
 * @param     plcd LCD硬件参数结构体(文件描述符 内存映射地址)
 * @param     fontPath 指向字体文件路径的字符串 例如 "./font/HYZhengYuan55W.ttf"
 * @param     canvasWidth 画布的宽度
 * @param     canvasHeight  画布的高度
 * @param     canvasColor   画布的底色 例如0xFF16D877
 * @param     text_x        文本在画布内的相对起始x坐标 可以理解为画布就是小屏幕 在画布内移动
 * @param     text_y        文本在画布内的相对起始y坐标
 * @param     fontSize 字体大小（像素） 例如72像素
 * @param     maxWidth      文本的最大宽度 即一行最大显示多大字符, 当和文本一样宽都是72时,则表示一行一个字, 垂直显示
 * @param     textColor     文本颜色
 * @param     text          要输出的文本字符串 "我爱中国"
 * @param     px            画布在LCD屏幕上的起始x坐标
 * @param     py            画布在LCD屏幕上的起始y坐标
 * @return
 *      @retval    true 显示成功
 *      @retval    false 显示失败
 * @date
 * @version   V1.0 2024年5月18日02:43:59 测试完成, 功能正常
 * @note
 * 前提:        LcdDev *lcd= lcd_init(LCD_PATH); // LCD的初始化
 * 使用 :
 *     displayText(lcd, FONT_PATH,
 *              画布宽, 画布高, 画布颜色getColor(A, R, G, B),
 *              字体偏移x, 字体偏移y, 字体大小, 一行显示多宽,
 *              字体颜色getColor(A, R, G, B), "显示内容",
 *              画布显示x坐标, 画布显示y坐标);
 */
bool displayText(LcdDev *plcd, char *fontPath,
                 u32 canvasWidth, u32 canvasHeight, unsigned int canvasColor,
                 s32 text_x, s32 text_y, s32 fontSize, s32 maxWidth,
                 unsigned int textColor, char *text,
                 int px, int py);
/**
 * @name      clearScreen_LCD
 * @brief     指定颜色清屏
 * @param     lcd LCD硬件参数结构体(文件描述符 内存映射地址)
 * @param     clear_color 要显示的颜色使用 getColor(0, 11, 21, 20)
 * @date
 * @version   V1.0   2024年5月18日16:22:24 测试后无误
 * @note
 * 前提:        LcdDev *lcd= lcd_init(LCD_PATH); // LCD的初始化
 * 使用 :      clearScreen_LCD(lcd, getColor(0, 11, 21, 20));
 */
void clearScreen_LCD(LcdDev *plcd, unsigned int clear_color);
#endif