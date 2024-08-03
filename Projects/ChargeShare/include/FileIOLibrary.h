#ifndef __FILEIOLIBRARY_H
#define __FILEIOLIBRARY_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include "jpeglib.h"
#include <linux/input.h>
// LCD设备的相关信息储存
typedef struct
{
    char *lcd_fd_Path;      // LCD设备驱动文件地址 "/dev/fb0"
    unsigned int LcdWidth;  // LCD的宽度
    unsigned int LcdHeight; // LCD的高度
    unsigned int LcdBpp;    // LCD的色深
} LcdInfo_t;

/**
 * @name      Get_Screeninfo_LCD
 * @brief     获取LCD屏幕硬件的宽度、高度、色深
 * @param     Manager 接收LCD屏幕信息
 * @param     lcdinfo [0]宽度 [1]高度 [2]色深
 * @return
 *      @retval    true 获取成功
 *      @retval    false 获取失败
 * @date      2024/05/11
 * @version   1.0
 * @note
 * 使用示范
 * LcdInfo_t lcdinfo;
 * lcdinfo.lcd_fd_Path = "/dev/fb0";
 * Get_Screeninfo_LCD(&lcdinfo);
 */
bool Get_Screeninfo_LCD(LcdInfo_t *Manager);

/**
 * @name      Clear_Screen_LCD
 * @brief     清屏函数, 刷新屏幕为指定颜色
 * @param     Manager 要刷新的屏幕信息结构体
 * @param     color 要刷新的颜色 0x00FFFFFF ARGB
 * @return
 *      @retval    true 清屏成功
 *      @retval    false 清屏失败
 * @date      2024/05/11
 * @version   1.0
 * @note
 * 使用示范 Clear_Screen_LCD(&lcdinfo, 0x00FF0000); 刷新为红色
 */
bool Clear_Screen_LCD(LcdInfo_t *Manager, int color);

/**
 * @name      Displays_GradientColorEffect_LCD
 * @brief     在LCD屏幕上渐变显示颜色, 最大可渐变16,581,375种颜色
 * @param     lcdinfo LCD屏幕信息;
 * @param     FrameRatedx 显示帧率24-60;
 * @param     SetShowTime 你要显示的时间长度(秒)
 * @param     step 颜色精度(1-255) 越低表示显示种数越多 最大可显示255^3种 (51表示216种 24表示1331种)
 * @return
 *      @retval    true 提前达到预定显示时间
 * @date      2024/05/12
 * @version   1.0
 * @note
 *      使用示范
 *       Displays_GradientColorEffect_LCD(&lcdinfo, FRAMERATED, 120, 24);
 *      显示一轮所用时间为y=((255/step +1)^3)/FrameRatedx 秒
 *      请注意一轮的显示时间长短
 */
bool Displays_GradientColorEffect_LCD(LcdInfo_t *lcdinfo, int FrameRatedx, unsigned long long SetShowTime, int step);

// BMP文件头:  取消字节对齐 用于对齐bmp头部结构
#pragma pack(1)

// 定义BMP头部数据结构 14字节, 有关于文件类型、文件大小、位图起始位置等信息
typedef struct
{
    unsigned short bfType;      // 文件标识(文件类型), 该值必须为字母ASCII码"BM" (0x4D42)
    unsigned int bfSize;        // 位图文件大小
    unsigned short bfReserved1; // 保留，必须为 0
    unsigned short bfReserved2; // 保留，必须为 0
    unsigned int bfOffBits;     // 文件开始到位图数据开始之间偏移量字节,因位图信息头和调色板的长度会根据不同情况而变化，用该偏移值可迅速读取到位数据。
} BITMAPFILEHEADER;

// 定义位图信息头40字节, 用于说明位图的尺寸等信息
typedef struct
{
    unsigned int biSize;         // 图像描述信息块的大小, 常为28H
    int biWidth;                 // ★图像宽度
    int biHeight;                // ★图像高度
    unsigned short biPlanes;     // 图像的plane总数, 恒为1
    unsigned short biBitCount;   // ★色深: 记录颜色的位数取值 [1(双色), 4,8,24,32]
    unsigned int biCompression;  // 数据压缩方式(0:不压缩; 1:8位压缩; 2:4位压缩)
    unsigned int biSizeImage;    // 图像区数据的大小, 必须是4的倍数
    int biXPelsPerMeter;         // 水平每米有多少像素, 在设备无关位图中, 填写00H
    int biYPelsPerMeter;         // 垂直每米有多少像素, 在设备无关位图中, 填写00H
    unsigned int biClrUsed;      // 此图像所用的颜色数, 不用-->固定为0
    unsigned int biClrImportant; // 重要颜色数, 不用-->固定为0
} BITMAPINFOHEADER;

// 54字节 24bit BMP文件数据结构。BMP文件的数据分别是位图文件头、位图信息头、调色板（24bit位图是没有的）、位图数据（RGB，实际储存为小端储存BGR 上下颠倒）
typedef struct
{
    BITMAPFILEHEADER bmpFileHeader; // 头部数据结构 14字节, 有关于文件类型、文件大小、位图起始位置等信息
    BITMAPINFOHEADER bmpInfoHeader; // 位图信息头   40字节, 用于说明位图的尺寸等信息
} BITMAPINFO;

#pragma pack()

/**
 * @name      BMP_Get_BitmapInfo
 * @brief     获取图片头部数据+位图信息, 可选[输出信息参数]
 * @param     BMP_Path 图像文件路径
 * @param     BitmapInfoManager 图片对应信息结构体
 * @return
 *      @retval    true 获取成功
 *      @retval    false 获取失败
 * @date      2024/05/12
 * @version   1.0
 * @note
 * 使用示范
 * BITMAPINFO jojoinfo;//定义图片信息结构体
 * BMP_Get_BitmapInfo("./jojo.bmp", &jojoinfo);
 */
bool BMP_Get_BitmapInfo(char *BMP_Path, BITMAPINFO *BitmapInfoManager);

/**
 * @name      BMP_Get_BitmapData
 * @brief     获取图片颜色分量数据
 * @param     BMP_Path 图像文件路径
 * @param     BitmapInfoManager 图片对应信息结构体
 * @param     bmpDataBuffer 存放图片颜色分量数据的数组空间
 * @return
 *      @retval    true 获取成功
 *      @retval    false 获取失败
 * @date      2024/05/12
 * @version   1.0
 * @note
 * 使用示范
 * BITMAPINFO jojoinfo;//定义图片信息结构体
 * char bmp_buf[jojoinfo.bmpInfoHeader.biWidth * jojoinfo.bmpInfoHeader.biHeight * (jojoinfo.bmpInfoHeader.biBitCount / 8)];
 * BMP_Get_BitmapData("./jojo.bmp", &jojoinfo, bmp_buf);
 */
bool BMP_Get_BitmapData(char *BMP_Path, BITMAPINFO const *BitmapInfoManager, char *bmpDataBuffer);

/**
 * @name      BMP_DisplayPicture_LCD
 * @brief     在LCD屏幕任意位置显示任意大小的bmp图片
 * @param     lcd_x             图像要在屏幕上显示的起始坐标x ↘ 左上角为0
 * @param     lcd_y             图像要在屏幕上显示的起始坐标y ↘ 左上角为0
 * @param     LcdManager        Lcd屏幕的基本信息
 * @param     bmpDataBuffer     存放图片颜色分量数据的数组空间
 * @param     BitmapInfoManager 图片对应信息结构体
 * @return
 *      @retval    true 显示成功
 *      @retval    false 显示失败
 * @date      2024/05/12
 * @version   1.0
 * @note
 * 使用示范
 * BMP_DisplayPicture_LCD(48, 48, &lcdinfo, bmp_buf, &jojoinfo);
 */
bool BMP_DisplayPicture_LCD(int lcd_x, int lcd_y, LcdInfo_t *LcdManager, char const *bmpDataBuffer, BITMAPINFO const *BitmapInfoManager);

/**
 * @name      BMP_Picture_Shrink
 * @brief     将图像缩小并生成新bmp图片
 * @param     BMP_Path 源图像文件路径
 * @param     New_BMP_Path 新图片的名称和路径 不写路径默认当前同级位置
 * @param     shrink_n 各边等比例缩小倍数
 * @return
 *      @retval    true 创建成功
 *      @retval    false 创建失败
 * @date      2024/05/14
 * @version   1.0
 * @note
 * 使用示范
 * Picture_Shrink_BMP("./hzw.bmp", "./1.bmp", 1);
 * 已测试不是4的倍数, 字节补齐, 260倍缩小, 3倍缩小
 */
bool BMP_Picture_Shrink(char *BMP_Path, char *New_BMP_Path, int shrink_n);

/**
 * @name      read_JPEG_file_LCD
 * @brief     在LCD屏幕上任意位置显示JPEG图像, 一次一张
 * @param     filename 图片路径
 * @param     start_x 起始位置 x
 * @param     start_y 起始位置 y
 * @param     lcd_mp  LCD设备驱动文件内存映射(mmap)后的起始地址
 * @param     lcdwidth  LCD屏幕的宽度 800
 * @return
 *      @retval    true  显示成功
 *      @retval    false 显示失败
 * @date      2024/05/14
 * @version   1.0
 * @note
 * 使用示范
 */
bool read_JPEG_file_LCD(char *filename, int start_x, int start_y, int *lcd_mp, int lcdwidth);

/**
 * @name      JPEG_ContinuousShow_LCD
 * @brief     在LCD屏幕上显示一个分帧后的连续图像
 * @param     StartNumber 起始序号
 * @param     EndNumber 结束序号
 * @param     PicName 图像名称
 * @param     PicPath 图像路径 ./gif/
 * @param     start_x 起始位置 x
 * @param     start_y 起始位置 y
 * @param     lcd_mp  LCD设备驱动文件内存映射(mmap)后的起始地址
 * @param     lcdwidth  LCD屏幕的宽度 800
 * @param     FrameRatedx 显示帧率24-60;
 * @return
 *      @retval    true  显示成功
 *      @retval    false 显示失败
 * @date      2024/05/14
 * @version   1.0
 * @note
 * 使用示范
 * EndNumber 必须大于 StartNumber且没有补0, 例如001,应该为Frame12.jpg Frame66.jpg
 */
void JPEG_ContinuousShow_LCD(int StartNumber, int EndNumber, char *PicName, char *PicPath, int start_x, int start_y, int *lcd_mp, int lcdwidth, int FrameRatedx);

#endif