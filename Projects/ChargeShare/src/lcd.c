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

#include "lcd.h"

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
LcdDev *lcd_init(const char *lcdpath)
{
    // 申请堆内存并定义一个记录LCD硬件信息的结构体指针, 这样被调函数结束后还存在
    LcdDev *lcd = (LcdDev *)calloc(1, sizeof(LcdDev));
    if (NULL == lcd)
    {
        printf("calloc for lcd failed\n");
        return NULL;
    }

    // 打开LCD 设备驱动文件
    lcd->fd = open(lcdpath, O_RDWR);
    if (-1 == lcd->fd)
    {
        printf("open lcd device error\n");
        return NULL;
    }

    // 对LCD进行内存映射
    lcd->mp = (int *)mmap(
        NULL,
        LCD_MAPSIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        lcd->fd,
        0);

    return lcd;
}

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
bool lcd_uninit(LcdDev *plcd)
{
    // 参数有效性检查
    if (NULL == plcd)
    {
        printf("argument is invailed\n");
        return false;
    }

    // 解除内存映射
    munmap(plcd->mp, LCD_MAPSIZE);

    // 关闭LCD屏
    close(plcd->fd);

    // 指针指向NULL，防止内存泄漏
    plcd = NULL;

    return true;
}

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
bool Show_Font_Lcd(LcdDev *plcd, char *HZKPath, int HZKsize, unsigned char font_buf[], int x, int y, unsigned int color)
{

    int HZKsizebytes = HZKsize * HZKsize / 8;
    // 打开汉字库文件  汉字库中的汉字都是16*16
    FILE *hzk_fp = fopen(HZKPath, "rb");
    if (NULL == hzk_fp)
    {
        perror("打开汉字库失败");
        return false;
    }
    /**计算待显示的汉字的在汉字库中的偏移量,并把光标偏移到该汉字的开头位置
     * font_buf[0]区号 ; -0xA0解码; *94一个区有94位 ; font_buf[1] 位号;-0xA0解码;
     */
    int offset = ((font_buf[0] - 0xA0 - 1) * 94 + (font_buf[1] - 0xA0 - 1)) * HZKsizebytes;
    fseek(hzk_fp, offset, SEEK_SET); // 移动到汉字库目标汉字位置

    // 从汉字库中读取该汉字的数据内容，需要读取32字节并存储在缓冲区
    unsigned char font[HZKsizebytes];
    fread(font, 1, HZKsizebytes, hzk_fp);

    // 根据汉字的每个点阵的状态来控制LCD相应的像素点显示未某种颜色  0xFFFFFFFF; //白色
    unsigned char data = 0;

    for (int h = 0; h < HZKsize; h++) // 控制点阵的行数
    {
        for (int w = 0; w < HZKsize / 8; w++) // 分析点阵的列数
        {
            data = font[h * 2 + w]; // 分析

            for (int i = 0; i < 8; i++) // 将汉字的基本构成点转换为像素 一个字节8位, 一次处理8个像素点
            {
                // 分析每个bit位的状态  1000 0000B 一次分析一位, 一次处理一个像素点
                if (data & 0x80)
                {
                    // 如果条件满足，则说明应该让LCD点亮一个像素点
                    *(plcd->mp + y * LCD_Width + x + 8 * w + i + LCD_Width * h) = color;
                }

                data <<= 1;
            }
        }
    }
    fclose(hzk_fp); // 关闭字库
    return true;
}

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
bool Show_zhString_Lcd(LcdDev *plcd, char *HZKPath, int HZKsize, unsigned char zhString_buf[], int x, int y, unsigned int color)
{
    int len = strlen((const char *)zhString_buf); // 计算用户输入的字符串长度
    int offset = 0;                               // 即将汉字的显示位置 ,一个汉字占2个字节

    for (int i = 0; i < len; i += 2) // 每次读取一个汉字,  每个汉字占两个字节
    {
        if (!Show_Font_Lcd(plcd, HZKPath, HZKsize, &zhString_buf[i], x + offset, y, color))
        {
            return false;
        }
        offset += HZKsize; // 每个汉字的宽度
    }

    return true;
}

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
                 int px, int py)
{

    // 加载字体  得到字体信息结构体的指针
    font *f = fontLoad(fontPath);
    if (!f)
    {
        perror("加载字体文件失败!");
        return NULL;
    }

    // 设置字体大小
    fontSetSize(f, fontSize);

    // 创建位图画布并初始化颜色
    bitmap *bm = createBitmapWithInit(canvasWidth, canvasHeight, 4, canvasColor);
    if (!bm)
    {
        perror("创建位图画布并初始化失败!");
        fontUnload(f); // 关闭字体库
        return false;
    }

    // 输出参数
    fontPrint(f, bm, text_x, text_y, text, textColor, maxWidth);
    show_font_to_lcd(plcd->mp, px, py, bm);

    // 结束函数
    fontUnload(f);
    destroyBitmap(bm);
    return true;
}

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
void clearScreen_LCD(LcdDev *plcd, unsigned int clear_color)
{
    for (int i = 0; i < (LCD_Width * LCD_Height); ++i)
    {
        plcd->mp[i] = clear_color;
    }
}
