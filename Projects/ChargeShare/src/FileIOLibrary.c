/**
 * @file name : FileIOLibrary.c
 * @brief     : 实现文件IO相关接口函数的封装
 * @author    : RISE_AND_GRIND@163.com
 * @date      : 2024/05/15
 * @version   : 1.3
 * @note      : 模块化实现
 * CopyRight (c)  2023-2024   RISE_AND_GRIND@163.com   All Right Reseverd
 */
#include "../include/FileIOLibrary.h"

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
bool Get_Screeninfo_LCD(LcdInfo_t *Manager)
{

    // 打开LCD 设备驱动 获取文件描述符 以读写权限 注意返回的是数值！
    int lcd_fd = open(Manager->lcd_fd_Path, O_RDWR);
    if (-1 == lcd_fd)
    {
        perror("打开LCD驱动文件失败");
        return false;
    }

    // 利用文件控制函数获取获取lcd参数信息， 获取可变参数 "cat /usr/include/linux/fb.h"
    // 定义一个结构体变量接收返回的信息
    struct fb_var_screeninfo vinfo;

    // 获取信息， 将信息写入上述的结构体变量中
    ioctl(lcd_fd, FBIOGET_VSCREENINFO, &vinfo);

    // 写入LCD的宽度，高度，色深
    Manager->LcdWidth = vinfo.xres;
    Manager->LcdHeight = vinfo.yres;
    Manager->LcdBpp = vinfo.bits_per_pixel;
    // 关闭lcd驱动文件
    close(lcd_fd);
    return true;
}

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
bool Clear_Screen_LCD(LcdInfo_t *Manager, int color)
{
    // 打开LCD设备驱动文件 读写方式
    int lcd_fd = open(Manager->lcd_fd_Path, O_RDWR);
    if (-1 == lcd_fd)
    {
        printf("打开lcd设备驱动文件失败!\n");
        return false;
    }

    // 对LCD进行内存映射  mmap函数 接收返回值
    int *lcd_mp = (int *)mmap(
        NULL,                                                           // 映射后的内存地址由内核自动分配
        Manager->LcdWidth * Manager->LcdHeight * (Manager->LcdBpp / 8), // 屏幕对应的字节数
        PROT_READ | PROT_WRITE,                                         // 可读可写
        MAP_SHARED,                                                     // 该映射的内存空间可共享
        lcd_fd,                                                         // 文件描述符
        0);                                                             // 偏移量0
    // 错误处理
    if (MAP_FAILED == lcd_mp)
    {
        printf("lcd设备驱动文件内存映射失败!\n");
        return false;
    }

    // 把颜色分量写入到申请的映射内存空间中，通过地址偏移实现访问
    for (int i = 0; i < Manager->LcdWidth * Manager->LcdHeight; ++i)
    {
        lcd_mp[i] = color;
    }

    // 关闭LCD并解除内存映射
    munmap(lcd_mp, Manager->LcdWidth * Manager->LcdHeight * (Manager->LcdBpp / 8));
    // 关闭LCD设备驱动文件
    close(lcd_fd);

    return true;
}

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
bool Displays_GradientColorEffect_LCD(LcdInfo_t *lcdinfo, int FrameRatedx, unsigned long long SetShowTime, int step)
{
    int SleepTime = 1000000 / FrameRatedx;                                        // 将帧率转换为微秒时间 帧间隔需要多少时间
    unsigned long long SetFrameRatedxCount = (SetShowTime * 1000000) / SleepTime; // 用户要求一共刷新多少帧
    // 显示精度 步进值 默认 51(step) 最大255 显示颜色数量为 ((255/step)+1)^3

    // 开始的颜色分量
    unsigned int startColor = 0;
    // 当前颜色分量
    unsigned int CurrentColor = 0;
    // 计算最大显示颜色数量
    unsigned long ShowCount = ((255 / step) + 1) * ((255 / step) + 1) * ((255 / step) + 1);
    // 计算一轮显示需要的显示时间 秒
    int ShowTime = ShowCount / FrameRatedx;

    // 解算得到颜色分量组成 ARGB
    // 控制红色见渐变
    int r = (startColor >> 16) & 0xFF;
    bool flagR = true; // true 表示颜色由深到浅色 false表示颜色由浅色到深色

    // 控制绿色见渐变
    int g = (startColor >> 8) & 0xFF;
    bool flagG = true; // true 表示颜色由深到浅色 false表示颜色由浅色到深色

    // 控制蓝色见渐变
    int b = startColor & 0xFF;
    bool flagB = true; // true 表示颜色由深到浅色 false表示颜色由浅色到深色
    printf("可以显示: %d 块色块,\n", (255 / step) + 1);
    printf("可以显示: %d 条色带\n", (((255 / step) + 1) * ((255 / step) + 1)));
    printf("可以显示: %lu 种颜色,\n", ShowCount);

    printf("每个色块内有 %d 条色带,\n", (255 / step) + 1);
    printf("每条色带内由 %d 种颜色组成,\n", (255 / step) + 1);
    printf("每个色块有 %d 种颜色,\n", ((255 / step) + 1)) * ((255 / step) + 1);

    printf("一次循环需要 %d秒 即 %d 分钟才能显示完成 \n", ShowTime, ShowTime / 60);

    while (1)
    {

        if (flagG)
        {
            for (g = 0x00; g <= 0xFF; g += step)
            {
                //               printf("\n");
                //               printf("新色块开始(%#x)\n", CurrentColor);

                if (flagB)
                {
                    for (b = 0x00; b <= 0xFF; b += step)
                    {
                        //                       printf("新色带:(%#x)\n", CurrentColor);
                        if (flagR)
                        {
                            // 00????
                            for (r = 0x00; r <= 0xFF; r += step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0xFF;
                            // FF????
                            flagR = !flagR;
                        }
                        else
                        {
                            // FF????
                            for (r = 0xFF; r >= 0x00; r -= step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0x00;
                            // 00????
                            flagR = !flagR;
                        }
                    }
                    b = 0xFF;
                    // ????FF
                    flagB = !flagB;
                }
                else
                {
                    // ????FF
                    for (b = 0xFF; b >= 0x00; b -= step) // 同色段渐变
                    {
                        //                        printf("新色带:(%#x)\n", CurrentColor);
                        if (flagR)
                        {
                            // 00????
                            for (r = 0x00; r <= 0xFF; r += step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0xFF;
                            // FF????
                            flagR = !flagR;
                        }
                        else
                        {
                            // FF????
                            for (r = 0xFF; r >= 0x00; r -= step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0x00;
                            // 00????
                            flagR = !flagR;
                        }
                    }
                    b = 0x00;
                    // ????00
                    flagB = !flagB;
                }
            }
            g = 0xFF;
            // ??FF??
            flagG = !flagG;
        }
        else
        {
            // ??FF??
            for (g = 0xFF; g >= 0x00; g -= step) // 同色段渐变
            {
                //               printf("\n");
                //             printf("新色块开始(%#x)\n", CurrentColor);
                if (flagB)
                {
                    for (b = 0x00; b <= 0xFF; b += step)
                    {
                        //          printf("新色带:(%#x)\n", CurrentColor);
                        if (flagR)
                        {
                            // 00????
                            for (r = 0x00; r <= 0xFF; r += step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0xFF;
                            // FF????
                            flagR = !flagR;
                        }
                        else
                        {
                            // FF????
                            for (r = 0xFF; r >= 0x00; r -= step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0x00;
                            // 00????
                            flagR = !flagR;
                        }
                    }
                    b = 0xFF;
                    // ????FF
                    flagB = !flagB;
                }
                else
                {
                    // ????FF
                    for (b = 0xFF; b >= 0x00; b -= step) // 同色段渐变
                    {
                        //                  printf("新色带:(%#x)\n", CurrentColor);
                        if (flagR)
                        {
                            // 00????
                            for (r = 0x00; r <= 0xFF; r += step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0xFF;
                            // FF????
                            flagR = !flagR;
                        }
                        else
                        {
                            // FF????
                            for (r = 0xFF; r >= 0x00; r -= step) // 同色段渐变
                            {
                                //               A              r           g        b
                                CurrentColor = (0x00 << 24) | (r << 16) | (g << 8) | b; // 生成颜色
                                Clear_Screen_LCD(lcdinfo, CurrentColor);
                                usleep(SleepTime);
                                // 达到要求时间, 退出程序
                                SetFrameRatedxCount--;
                                if (SetFrameRatedxCount == 0)
                                {
                                    return true;
                                }
                            }
                            r = 0x00;
                            // 00????
                            flagR = !flagR;
                        }
                    }
                    b = 0x00;
                    // ????00
                    flagB = !flagB;
                }
            }
            g = 0x00;
            // ??00??
            flagG = !flagG;
        }
    }
}

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
bool BMP_Get_BitmapInfo(char *BMP_Path, BITMAPINFO *BitmapInfoManager)
{
    // 打开待获取的BMP图像文件
    FILE *bmp_fp = fopen(BMP_Path, "rb");
    if (NULL == bmp_fp)
    {
        perror("打开待获取的BMP图像文件失败");
        return false;
    }

    // 读取BMP文件的图像信息
    fread(BitmapInfoManager, 1, 54, bmp_fp); // 读取54字节
#if 0
    printf("bfType\t\t%#X\n", BitmapInfoManager->bmpFileHeader.bfType);
    printf("bfSize\t\t%u\n", BitmapInfoManager->bmpFileHeader.bfSize);
    printf("bfReserved1\t%hu\n", BitmapInfoManager->bmpFileHeader.bfReserved1);
    printf("bfReserved2\t%hu\n", BitmapInfoManager->bmpFileHeader.bfReserved2);
    printf("bfOffBits\t%u\n", BitmapInfoManager->bmpFileHeader.bfOffBits);
    printf("\n");
    printf("biSize\t\t%#X\n", BitmapInfoManager->bmpInfoHeader.biSize);
    printf("biWidth\t\t%d\n", BitmapInfoManager->bmpInfoHeader.biWidth);
    printf("biHeight\t%d\n", BitmapInfoManager->bmpInfoHeader.biHeight);
    printf("biPlanes\t%hu\n", BitmapInfoManager->bmpInfoHeader.biPlanes);
    printf("biBitCount\t%hu\n", BitmapInfoManager->bmpInfoHeader.biBitCount);
    printf("biCompression\t%u\n", BitmapInfoManager->bmpInfoHeader.biCompression);
    printf("biSizeImage\t%u\n", BitmapInfoManager->bmpInfoHeader.biSizeImage);
    printf("biXPelsPerMeter\t%#X\n", BitmapInfoManager->bmpInfoHeader.biXPelsPerMeter);
    printf("biYPelsPerMeter\t%#X\n", BitmapInfoManager->bmpInfoHeader.biYPelsPerMeter);
    printf("biClrUsed\t%u\n", BitmapInfoManager->bmpInfoHeader.biClrUsed);
    printf("biClrImportant\t%u\n", BitmapInfoManager->bmpInfoHeader.biClrImportant);
#endif
    // 关闭BMP
    fclose(bmp_fp);
    return true;
}

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
bool BMP_Get_BitmapData(char *BMP_Path, BITMAPINFO const *BitmapInfoManager, char *bmpDataBuffer)
{
    // 打开待获取的BMP图像文件
    FILE *bmp_fp = fopen(BMP_Path, "rb");
    if (NULL == bmp_fp)
    {
        perror("打开待获取的BMP图像文件失败");
        return false;
    }

    // 读取BMP图*片的颜色分量  图宽*图高*每个像素的字节数
    fseek(bmp_fp, 54, SEEK_SET); // 跳过头部信息
    fread(bmpDataBuffer, 1, BitmapInfoManager->bmpInfoHeader.biWidth * BitmapInfoManager->bmpInfoHeader.biHeight * (BitmapInfoManager->bmpInfoHeader.biBitCount / 8), bmp_fp);

    // 关闭BMP
    fclose(bmp_fp);
    return true;
}

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
bool BMP_DisplayPicture_LCD(int lcd_x, int lcd_y, LcdInfo_t *LcdManager, char const *bmpDataBuffer, BITMAPINFO const *BitmapInfoManager)
{
    int i = 0;                                             // 公用, 用于遍历数组计数
    int bmp_w = BitmapInfoManager->bmpInfoHeader.biWidth;  // 图像的宽度
    int bmp_h = BitmapInfoManager->bmpInfoHeader.biHeight; // 图像的高度
    int scrn_w = LcdManager->LcdWidth;                     // 屏幕宽度
    int scrn_h = LcdManager->LcdHeight;                    // 屏幕高度
    int scrn_bit_width = LcdManager->LcdBpp / 8;           // 屏幕位深(字节形式)

    int lcd_fd = open(LcdManager->lcd_fd_Path, O_RDWR);
    if (-1 == lcd_fd)
    {
        perror("打开lcd设备驱动文件失败!");
        return false;
    }

    // 对LCD进行内存映射  mmap函数 接收返回值
    int *lcd_mp = (int *)mmap(
        NULL,                             // 映射后的内存地址由内核自动分配
        scrn_w * scrn_h * scrn_bit_width, // 屏幕对应的字节数
        PROT_READ | PROT_WRITE,           // 可读可写
        MAP_SHARED,                       // 该映射的内存空间可共享
        lcd_fd,                           // 文件描述符
        0);                               // 偏移量0
    // 错误处理
    if (MAP_FAILED == lcd_mp)
    {
        perror("lcd设备驱动文件内存映射失败!");
        return false;
    }

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     * 在任意位置,显示任意大小的图片
     **+++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    // 校准任意位置(lcd_x, lcd_y),在文件起始位置上偏移, (lcd_x, lcd_y)是偏移量, 新的图像左上角0位置实际是
    // 将图像显示起点调整到要求的实际屏幕位置
    /*
     *****
     *▲***
     *****
     */
    lcd_mp = lcd_mp + lcd_x; // 移动x轴的位置

    lcd_mp = lcd_mp + lcd_y * scrn_w; // 移动y轴的位置

    // 要写入图片的任意大小, 即要操作的内存范围(bmp_w, bmp_h)
    int x = 0, y = 0;                  // 实时写入的像素点位置
    int data = 0;                      // 当前像素点
    for (y = 0, i = 0; y < bmp_h; y++) // 图像纵向像素宽度
    {
        for (x = 0; x < bmp_w; x++) // 图像横向像素宽度
        {
            // 校准BMP像素点位置
            i = 3 * (y * bmp_w + x);
            // 将BMP的像素点BGR转换为LCD所需的ARGB
            data = (0xFF << 24) | (bmpDataBuffer[i + 2] << 16) | (bmpDataBuffer[i + 1] << 8) | (bmpDataBuffer[i] << 0);
            //           LCD距离尾部高度,高度越来越高
            *(lcd_mp + (bmp_h - y - 1) * scrn_w + x) = data; // 这里的scrn_w是设备的宽度, lcd_buf起始-->终点 <==>显示文件的位置从终点--起点,
        }
    }

    // 关闭LCD并解除内存映射
    munmap(lcd_mp, scrn_w * scrn_h * scrn_bit_width);
    // 关闭LCD设备驱动文件
    close(lcd_fd);
    return true;
}

#if 0
/**
 * @name      Picture_Shrink
 * @brief     将图像缩小并生成新图片
 * @param     BMP_Path 源图像文件路径
 * @param     New_BMP_Path 新图片的名称和路径 不写路径默认当前同级位置
 * @param
 * @param
 * @param
 * @param
 * @return
 *      @retval    true 创建成功
 *      @retval    false 创建失败
 * @date      2024/05/13
 * @version   1.0
 * @note
 * 使用示范
 * DisplayPicture_Screen(48, 48, &lcdinfo, bmp_buf, &jojoinfo);
 */
bool Picture_Shrink_BMP(char *BMP_Path, char *New_BMP_Path, int shrink_n)
{
    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     * 源图片信息获取
     **+++++++++++++++++++++++++++++++++++++++++++++++++++++++**/

    int width = 0;                // 源图片宽度
    int height = 0;               // 源图片高度
    BITMAPINFO BitmapInfoManager; // 源图片的信息结构体
    // 源图像文件指针
    FILE *bmp_fp = fopen(BMP_Path, "rb");
    if (NULL == bmp_fp)
    {
        perror("打开源图片失败");
        return false;
    }
    // 读取源图片的图像头信息 54字节
    if (fread(&BitmapInfoManager, 1, 54, bmp_fp) <= 0)
    {
        perror("读取源图片的图像头信息失败");
        return false;
    }

    width = BitmapInfoManager.bmpInfoHeader.biWidth;
    height = BitmapInfoManager.bmpInfoHeader.biHeight;

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     * 新图片操作
     **+++++++++++++++++++++++++++++++++++++++++++++++++++++++**/

    // 在指定路径创建新图片(空文件)        "二进制只写"
    FILE *newbmp_fp = fopen(New_BMP_Path, "wb");
    if (NULL == newbmp_fp)
    {
        perror("创建新的BMP图像文件失败");
        return false;
    }
    // 修改为源图像信息为新图像信息
    BitmapInfoManager.bmpFileHeader.bfSize = (height / shrink_n * width / shrink_n) * 3 + 54;
    BitmapInfoManager.bmpInfoHeader.biHeight = height / shrink_n;
    BitmapInfoManager.bmpInfoHeader.biWidth = width / shrink_n;
    BitmapInfoManager.bmpInfoHeader.biSizeImage = BitmapInfoManager.bmpInfoHeader.biSizeImage / shrink_n / shrink_n;
    // 写入新数据图像信息
    if (fwrite(&BitmapInfoManager, 1, 54, newbmp_fp) <= 0)
    {
        perror("写入新数据图像信息失败");
        return false;
    }
    // 每次从源图片中读取一行像素点的颜色分量的大小
    int line_size = width * 3;           // 源图像一行字节数: 一个像素点有3字节
    char src_pbuf_xiangshudian[3] = {0}; // 缓冲区: 从源图像中读行数据的缓冲区

    // 更新新图片数据: 循环从源图片中读取一行图像数据, 并写入到新图片中
    int line_num = height; // 还有多少行未处理
    int i = 0;             // 该行已处理像素点数量
    int j = shrink_n - 1;  // 每次跳过的行数
    while (line_num > 0)
    {
        //  行内操作
        for (i = 0; i < width; i += shrink_n)
        {
            // 行内跳跃
            fseek(bmp_fp, (shrink_n - 1) * 3, SEEK_CUR);
            if (fread(src_pbuf_xiangshudian, 1, 3, bmp_fp) <= 0) // 读取1个像素点 3字节
            {
                perror("读源图片像素点数据失败");
                return false;
            }

            if (fwrite(src_pbuf_xiangshudian, 3, 1, newbmp_fp) <= 0)
            {
                perror("读向新图像中写入新行数据失败");
                return false;
            }
        }
        // 高度跳跃
        fseek(bmp_fp, j * line_size, SEEK_CUR);
        line_num -= shrink_n; // 2是采样距离即缩小倍数
    }

    return true;
}
#endif

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
bool BMP_Picture_Shrink(char *BMP_Path, char *New_BMP_Path, int shrink_n)
{
    int b = 1;
    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     * 源图片信息获取
     **+++++++++++++++++++++++++++++++++++++++++++++++++++++++**/

    int width = 0;                // 源图片宽度
    int height = 0;               // 源图片高度
    BITMAPINFO BitmapInfoManager; // 源图片的信息结构体
    // 源图像文件指针
    FILE *bmp_fp = fopen(BMP_Path, "rb");
    if (NULL == bmp_fp)
    {
        perror("打开源图片失败");
        return false;
    }

    // 读取源图片的图像头信息 54字节
    if (fread(&BitmapInfoManager, 1, sizeof(BITMAPINFO), bmp_fp) != sizeof(BITMAPINFO))
    {
        perror("读取源图片的图像头信息失败");
        fclose(bmp_fp);
        return false;
    }

    width = BitmapInfoManager.bmpInfoHeader.biWidth;
    height = BitmapInfoManager.bmpInfoHeader.biHeight;

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     * 新图片操作
     **+++++++++++++++++++++++++++++++++++++++++++++++++++++++**/

    // 在指定路径创建新图片(空文件) "二进制只写"
    FILE *newbmp_fp = fopen(New_BMP_Path, "wb");
    if (NULL == newbmp_fp)
    {
        perror("创建新的BMP图像文件失败");
        fclose(bmp_fp);
        return false;
    }

    // 修改为源图像信息为新图像信息
    BitmapInfoManager.bmpFileHeader.bfSize = ((height / shrink_n * ((width / shrink_n * 3 + 3) & ~3)) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER));
    BitmapInfoManager.bmpInfoHeader.biHeight = height / shrink_n;
    BitmapInfoManager.bmpInfoHeader.biWidth = width / shrink_n;
    BitmapInfoManager.bmpInfoHeader.biSizeImage = (height / shrink_n * ((width / shrink_n * 3 + 3) & ~3));

    // 写入新数据图像信息
    if (fwrite(&BitmapInfoManager, 1, sizeof(BITMAPINFO), newbmp_fp) != sizeof(BITMAPINFO))
    {
        perror("写入新数据图像信息失败");
        fclose(bmp_fp);
        fclose(newbmp_fp);
        return false;
    }
    // printf("调试 %d\n", b++);
    // 每次从源图片中读取一行像素点的颜色分量的大小
    int srcLineSize = (width * 3 + 3) & ~3;                                     // 源图像一行字节数: 一个像素点有3字节，行对齐到4字节
    int newLineSize = ((BitmapInfoManager.bmpInfoHeader.biWidth) * 3 + 3) & ~3; // 新图像一行字节数

    char *srcBuffer = (char *)malloc(srcLineSize);
    if (srcBuffer == NULL)
    {
        perror("申请srcBuffer失败");
        fclose(bmp_fp);
        fclose(newbmp_fp);
        return false;
    }

    // printf("x调试 2 newLineSize=%d\n", newLineSize);
    char *newBuffer = (char *)malloc(newLineSize + 2);

    //  printf("x调试 3\n");
    if (newBuffer == NULL)
    {
        perror("申请newBuffer失败");
        fclose(bmp_fp);
        fclose(newbmp_fp);
        free(srcBuffer);
        return false;
    }
    //  printf("x调试 3\n");

    int x = 0;
    // 更新新图片数据: 循环从源图片中读取一行图像数据, 并写入到新图片中
    for (int y = 0; y < height; y += shrink_n)
    {
        fseek(bmp_fp, sizeof(BITMAPINFO) + y * srcLineSize, SEEK_SET);
        if (fread(srcBuffer, 1, srcLineSize, bmp_fp) != srcLineSize)
        {
            perror("读源图片像素点数据失败");
            fclose(bmp_fp);
            fclose(newbmp_fp);
            free(srcBuffer);
            free(newBuffer);
            return false;
        }

        for (x = 0; x < width; x += shrink_n)
        {

            // printf("newBuffer * 3=%d newBuffer * 3+1=%d newBuffer * 3+2=%d newLineSize=%d \n", (x / shrink_n) * 3, (x / shrink_n) * 3 + 1, (x / shrink_n) * 3 + 2, newLineSize + 2);
            newBuffer[(x / shrink_n) * 3] = srcBuffer[x * 3];

            newBuffer[(x / shrink_n) * 3 + 1] = srcBuffer[x * 3 + 1];
            newBuffer[(x / shrink_n) * 3 + 2] = srcBuffer[x * 3 + 2];
        }

        fseek(newbmp_fp, sizeof(BITMAPINFO) + (y / shrink_n) * newLineSize, SEEK_SET);
        if (fwrite(newBuffer, 1, newLineSize, newbmp_fp) != newLineSize)
        {
            perror("向新图像中写入新行数据失败");
            fclose(bmp_fp);
            fclose(newbmp_fp);
            free(srcBuffer);
            free(newBuffer);
            return false;
        }
    }

    fclose(bmp_fp);
    fclose(newbmp_fp);
    free(srcBuffer);
    free(newBuffer);

    return true;
}

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
bool read_JPEG_file_LCD(char *filename, int start_x, int start_y, int *lcd_mp, int lcdwidth)
{

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     * JPEG图像解码操作
     **+++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    struct jpeg_decompress_struct cinfo; // jpeg解压后信息存放的结构体
    struct jpeg_error_mgr jerr;          // jpeg处理错误信息存放的结构体, 注意要jerr和cinfo的生命周期一样长
    FILE *infile;                        // 源文件 文件指针
    unsigned char *buffer;               // jpeg输出缓冲区
    int row_stride;                      /* 解码后输出图像的一行宽度*/

    // 打开文件要解码的JPEG图像  二进制方式
    if ((infile = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "不能打开 %s\n", filename);
        return false;
    }

    /* Step 1: 分配和初始化JPEG解压缩对象 ,设置正常的JPEG错误例程，然后重写error_exit. */
    cinfo.err = jpeg_std_error(&jerr);

    /* 初始化JPEG解压缩对象*/
    jpeg_create_decompress(&cinfo);

    /* Step 2: 指定数据源 (eg, a file) */
    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: 使用jpeg_read_header()读取文件参数 */
    /*可以忽略jpeg_read_header()的返回值,因为stdio数据源不支持暂停, 传递TRUE来拒绝只包含表的JPEG文件作为错误。更多信息请参阅libjpeg.txt。*/
    (void)jpeg_read_header(&cinfo, TRUE);

    /* Step 4: 设置解压参数 */
    /* 在这个示例中，我们不需要更改jpeg_read_header()设置的任何默认值，因此这里什么都不做。*/

    /* Step 5: 开始解压 */
    /* 可以忽略返回值，因为在stdio数据源中不能暂停。*/
    (void)jpeg_start_decompress(&cinfo);

    /*
     * 在读取数据之前，我们可能需要做一些自己的设置。在jpeg_start_decompress()之后，我们有了正确的缩放后的输出图像尺寸，
     * 以及如果我们要求颜色量化的话输出的颜色映射。在这个例子中，我们需要创建一个大小合适的输出工作缓冲区。
     */
    /* 在输出缓冲区中对每一行进行采样 */
    row_stride = cinfo.output_width * cinfo.output_components; // 计算解码后图像的一行的大小

    /* 创建完整一行的样本数组，该数组在处理image时将消失 */
    buffer = calloc(1, row_stride);

    /* Step 6: while (行扫描有待读取) */
    /*           jpeg_read_scanlines(...); */

    /* 这里我们使用库的状态变量cinfo。Output_scanline作为循环计数器，这样我们就不必跟踪自己。*/

    /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     * LCD屏幕显示操作
     **+++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    int data = 0;                                       // LCD屏幕每个ARGB像素点的颜色分量
    while (cinfo.output_scanline < cinfo.output_height) // 解码jpeg图像的行扫描所在高度和该图像完整高度
    {
        /* Jpeg_read_scanlines需要一个指向行扫描的指针数组。这里的数组只有一个元素长，你可以一次请求多个扫描线。*/
        (void)jpeg_read_scanlines(&cinfo, &buffer, 1); // 读取JPEG解码后的一行数据 从上到下，从左到右  RGB RGB RGB RGB

        for (int i = 0; i < cinfo.output_width; ++i) // 将解码后的一行像素信息传递到LCD屏幕中显示, 由于JPEG和LCD的显示是一样的都是大端储存 ,故而 (JPEG)RGB-->(LCD)ARGB
        {
            // 生成LCD新像素点信息
            data |= buffer[3 * i] << 16;    // R
            data |= buffer[3 * i + 1] << 8; // G
            data |= buffer[3 * i + 2];      // B

            // 把像素点写入到LCD的指定位置
            lcd_mp[lcdwidth * start_y + start_x + lcdwidth * (cinfo.output_scanline - 1) + i] = data;

            data = 0;
        }
    }

    /* Step 7: 完成解码 */
    /*我们可以忽略返回值，因为数据源不支持暂停 */
    (void)jpeg_finish_decompress(&cinfo);

    /* Step 8: 释放JPEG解压缩对象 */
    /* 这是一个重要的步骤，因为它将释放大量内存 */
    jpeg_destroy_decompress(&cinfo);

    /*
     * 在finish_decompress之后，我们可以关闭输入文件。在这里，我们将其推迟到不可能再有JPEG错误之后，
     * 以便简化上面的setjmp错误逻辑。(实际上，我不认为jpeg_destroy可以做一个错误退出，但为什么要假设任何事情…
     */
    // 关闭打开文件要解码的JPEG图像
    fclose(infile);

    /* 在这一点上，你可能想要检查是否发生了任何损坏数据的警告(测试jerr.pub. js是否存在错误)。Num_warnings是非零的)。 */
    return true;
}

/**
 * @name      JPEG_ContinuousShow_LCD
 * @brief     在LCD屏幕上显示一个分帧后的连续图像或指定区间图像
 * @param     StartNumber 起始序号
 * @param     EndNumber 结束序号
 * @param     PicPath 图像路径 ./gif/   ./gif/Frame%d.jpg
 * @param     PicName 图像名称 Frame
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
void JPEG_ContinuousShow_LCD(int StartNumber, int EndNumber, char *PicPath, char *PicName, int start_x, int start_y, int *lcd_mp, int lcdwidth, int FrameRatedx)
{
    int SleepTime = 1000000 / FrameRatedx;        // 将帧率转换为微秒时间 帧间隔需要多少时间
    int PictureNum = EndNumber - StartNumber;     // 计算要显示图片的数量
    char gif_path[128] = {0};                     // 构造图片路径的缓存区
    for (int i = StartNumber; i < EndNumber; ++i) //    for (int i = StartNumber; i < PictureNum; ++i)
    {
        sprintf(gif_path, "%s%s%d.jpg", PicPath, PicName, i);           // 构造jpg图片的路径
        read_JPEG_file_LCD(gif_path, start_x, start_y, lcd_mp, lcdwidth); // 在LCD上显示
        usleep(SleepTime);
    }
}
