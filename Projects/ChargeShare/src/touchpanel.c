/**
 * @file name : TouchPanel.c
 * @brief     : 触摸屏相关功能函数
 * @author    : RISE_AND_GRIND@163.com
 * @date      : 2024/05/18
 * @version   : 1.0
 * @note      :
 *              默认800*480分辨率
 *              编译命令 arm-linux-gcc main.c ./src/LCD.c -o ./demo.out -I ./include -L ./lib -ljpeg
 * CopyRight (c)  2023-2024   RISE_AND_GRIND@163.com   All Right Reseverd
 */

#include "../include/touchpanel.h"

/**
 * @name      ts_init
 * @brief     触摸屏的初始化
 * @param     tspath : 指的是触摸屏的设备路径
 * @date      2024/05/18
 * @version   1.0
 * @note
 * 必须要有:
 *
 */
int ts_init(const char *tspath)
{
    // 打开触摸屏
    int ts_fd = open(tspath, O_RDWR);

    if (-1 == ts_fd)
    {
        printf("open ts device error\n");
        return -1;
    }

    return ts_fd;
}

#if 0
/**
 * @name      GetTouchVal_Once
 * @brief     获取一次触摸屏的按下坐标
 * @param     ts_fd open触摸屏驱动文件的文件描述符
 * @param     ts_event 输入设备的信息结构体(这里是触摸屏)
 * @param     TouchScreen_x 存放触摸屏x坐标
 * @param     TouchScreen_y 存放触摸屏x坐标
 * @return
 * @date      2024/05/14
 * @version   1.0
 * @note
 * 必须要有:
 *      // 1.打开触摸屏
 *          int ts_fd = open("/dev/input/event0", O_RDWR);
 *          int x = 0, y = 0;
 *      // 2.读取输入设备的信息
 *          struct input_event ts_event;
 *
 *           close(ts_fd);
 *
 */
void GetTouchVal_Once(int ts_fd, struct input_event *ts_event, int *TouchScreen_x, int *TouchScreen_y)
{
    static int TouchScreen_cnt = 0;
    //| O_NONBLOCK 要以非阻塞状态打开,否则会一定概率卡在某个位置等待新数据到来
    /* 例如 int ts_fd = open("/dev/input/event0", O_RDWR | O_NONBLOCK);*/
    read(ts_fd, ts_event, sizeof(struct input_event));

    // 3.分析读取的设备信息 (type + code + value)
    if (ts_event->type == EV_ABS) // 说明是触摸屏
    {

        if (ts_event->code == ABS_X) // 说明是X轴
        {
            TouchScreen_cnt++;
            *TouchScreen_x = ts_event->value * 800 / 1024; // 触摸屏适配显示屏分辨率
        }

        if (ts_event->code == ABS_Y) // 说明是Y轴
        {
            TouchScreen_cnt++;
            *TouchScreen_y = ts_event->value * 480 / 600; // 触摸屏适配显示屏分辨率
        }

        if (TouchScreen_cnt >= 2) // 消除电容值变化导致的按下离开的抖动 电容变化比读取快 会导致重复显示
        {
            printf("TouchScreen coordinate = (%d,%d)\n", *TouchScreen_x, *TouchScreen_y); // 输出x y轴坐标
            TouchScreen_cnt = 0;
        }
    }
}
#elif 1
/**
 * @name      GetTouchVal_Once
 * @brief     获取一次触摸屏的按下坐标, 获取有效值为用户松开的位置
 * @param     ts_fd : 指的是触摸屏的文件描述符
 * @param     ts_x  : 指的是用于存储X轴坐标的变量地址
 * @param     ts_y  : 指的是用于存储X轴坐标的变量地址
 * @return
 * @date      2024/05/18
 * @version   1.0 2024年5月18日13:49:02 基本功能实现, 并仅获取松手坐标
 * @note
 * 必须要有:
 *         该函数每次只能获取一次触摸屏坐标，并会把触摸屏的坐标值分别存储到传递进来的两个变量地址下
 *
 *          调用该函数之前，必须提前调用ts_init()对触摸屏进行初始化,否则调用会失败
 *          x y 是全局变量
 *          注意获取后坐标没有清空, 请调用后将坐标清空为 -1 -1
 */
void GetTouchVal_Once(int ts_fd, int *ts_x, int *ts_y)
{

    int cnt = 0;
    // 1.读取输入设备的信息
    struct input_event ts_event;
    static int ts_x_tmp = -1;
    static int ts_y_tmp = -1;
    // 2.循环获取坐标，当获取到完整坐标(x,y)则终止循环
    while (1)
    {
        // read函数默认会阻塞的，也就是没有读取数据，则卡这句话
        read(ts_fd, &ts_event, sizeof(ts_event));

        // 3.分析读取的设备信息 (type + code + value)
        if (ts_event.type == EV_ABS && ts_event.code == ABS_X) // 说明是触摸屏X轴
        {
            cnt++;
            ts_x_tmp = ts_event.value * 800 / 1024;
        }
        if (ts_event.type == EV_ABS && ts_event.code == ABS_Y) // 说明是触摸屏Y轴
        {
            cnt++;
            ts_y_tmp = ts_event.value * 480 / 600;
        }

        // 如果获取完整坐标，则输出获取的坐标用于项目测试
        if (ts_event.type == 1 && ts_event.code == BTN_TOUCH && ts_event.value == 0 && cnt >= 2)
        {
            *ts_x = ts_x_tmp;
            *ts_y = ts_y_tmp;
            printf("触摸点坐标 = (%d,%d)\n", *ts_x, *ts_y); // 输出x y轴坐标
            break;
        }
    }
}

#endif