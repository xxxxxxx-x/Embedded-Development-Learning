#ifndef _CHARGER_H
#define _CHARGER_H
/**
 * @file name : charger.c
 * @brief     : charging充电站 充电相关功能
 * @author    : RISE_AND_GRIND@163.com
 * @date      : 2024/05/21
 * @version   : 1.0
 * @note      :
 *              默认800*480分辨率
 *              编译命令 arm-linux-gcc main.c ./src/* -o ./demo.out -I ./include -L ./lib -ljpeg -lfont  -lm   -pthread
 * CopyRight (c)  2023-2024   RISE_AND_GRIND@163.com   All Right Reseverd
 */
/***************************************头文件***************************************/
/***************************************头文件***************************************/
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "./font.h"
#include "./FileIOLibrary.h"
#include "./lcd.h"
#include "./touchpanel.h"
/// 本c要用
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for usleep function
#include <termios.h>
#include "./charging.h"
#include "./usermanagement.h"
/***************************************END******************************************/
/***************************************预处理***************************************/

/***************************************END******************************************/
/***************************************数据类型***************************************/
// 充电枪结构体 枪名称 最大功率 电压 电流
typedef struct
{
    char model[20]; // 充电枪名称
    float voltage;  // 充电电压 (V)
    float current;  // 充电电流 (A)
    float maxPower; // 最大充电功率 (kW)
} Charger;
// 车辆信息结构体 名称 电池容量 当前容量
typedef struct
{
    char name[20];         // 车辆信息
    float batteryCapacity; // 电池容量 (kWh)
    float currentCharge;   // 当前电量 (kWh) 已经有的电量
} Car;
// 充电信息结构体
typedef struct
{
    Car car;                // 选择的车辆所有的信息
    Charger charger;        // 选择的充电枪所有的信息
    float chargedEnergy;    // 全部充入电量 (kWh)△
    float chargingCost;     // 全部的充电费用 (元) 费用结算
    float chargePercentage; // 已充电百分比 (%)
    time_t startTime;       // 充电开始时间
    int useTime;            // 已经使用的时间(s)
} CharSession;
/***************************************END******************************************/
// 充电价格计算
/**
 * @name      calculateChargingCost
 * @brief     充电价格计算
 * @param     energy 充入电量 (kWh)
 * @param     isPeak 是否为高峰时段 (1 表示高峰时段, 0 表示非高峰时段)
 * @param     isMember 是否为会员 (1 表示会员, 0 表示非会员)
 * @date      2024/05/21
 * @version   1.0
 * @note      由于前面没有考虑到是否会员,故而用户信息结构体内无身份标识, 时间有限,暂时不改
 *            用户余额>=3000元是会员,<=3000元不是会员
 */
float calculateChargingCost(float energy, int isPeak, int isMember);
// 计算剩余充电时间的函数
/**
 * @name      calculateEstimatedEndTime
 * @brief     计算剩余充电时间的函数, 将计算结果转化为字符串
 * @param     totalCapacity 汽车总电池容量 (kWh)
 * @param     currentCharge 已经充满的电量 (kWh)
 * @param     power 充电功率 (kW)
 * @param     str 接收剩余时间的字符串数组
 * @return    返回充满电需要多少分钟
 * @date      2024/05/21
 * @version   1.0
 * @note      转后的结果为: 01小时06分钟00秒
 */
float calculateEstimatedEndTime(float totalCapacity, float currentCharge, float power, char *str);
// 生成指定范围内随机浮点数的函数
/**
 * @name      randomFloat
 * @brief     生成指定范围内随机浮点数的函数
 * @param     min  随机数的最小值
 * @param     max  随机数的最大值
 * @return
 *      @retval 生成的随机浮点数，范围在 [min, max]
 * @date      2024/05/21
 * @version   1.0
 * @note
 */
float randomFloat(float min, float max);
/*******************************遍历打印*******************************/
// 打印 CharSession 结构体内容的函数 汽车信息, 调试用
void printChargingSession(const CharSession *session);
#endif