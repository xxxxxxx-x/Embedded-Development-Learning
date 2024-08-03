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

#include "../include/charger.h"
/**************************************全局变量**************************************/

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
float calculateChargingCost(float energy, int isPeak, int isMember)
{
    // 根据是否为高峰时段设置基础电费率
    // 高峰时段费率为 1.20 元/kWh, 非高峰时段费率为 0.80 元/kWh
    float rate = isPeak ? 1.20 : 0.80;
    // 如果是会员，享受 10% 的折扣
    if (isMember)
    {
        rate *= 0.9; // 会员折扣后的费率
    }
    // 计算总费用，包括每次充电的固定服务费 2.00 元
    return energy * rate + 2.00; // 总费用 = 充电电量 * 费率 + 服务费
}

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
float calculateEstimatedEndTime(float totalCapacity, float currentCharge, float power, char *str)
{
    printf("\n进入计算剩余充电时间的函数模块\n");
    // 计算剩余需要充入的电量
    float remainingEnergy = totalCapacity - currentCharge;

    // 计算充电所需的总小时数
    float hours = remainingEnergy / power;
    int totalSeconds = (int)(hours * 3600); // 得到秒数

    // 计算小时、分钟和秒
    int hoursPart = totalSeconds / 3600;
    int minutesPart = (totalSeconds % 3600) / 60;
    int secondsPart = totalSeconds % 60;

    // 格式化字符串，将结果存储在传入的字符串数组 str 中
    snprintf(str, 50, "%02d小时%02d分钟%02d秒", hoursPart, minutesPart, secondsPart);
    printf("退出计算剩余充电时间的函数模块\n");
#if TESTINGMODE
    return (totalSeconds / 60);
#elif PRACTICALMODE
    return totalSeconds;
#endif
}

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
float randomFloat(float min, float max)
{
    // 生成一个 [0, 1) 范围内的随机浮点数，然后缩放到 [min, max] 范围内
    return min + ((float)rand() / RAND_MAX) * (max - min);
}
/*******************************遍历打印*******************************/
// 打印 ChargingSession 结构体内容的函数 汽车信息, 调试用
void printChargingSession(const CharSession *session)
{
    // 打印车辆信息
    printf("车辆信息:\n");
    printf("  名称: %s\n", session->car.name);
    printf("  电池容量: %.2f kWh\n", session->car.batteryCapacity);
    printf("  当前电量: %.2f kWh\n\n", session->car.currentCharge);

    // 打印充电枪信息
    printf("充电枪信息:\n");
    printf("  名称: %s\n", session->charger.model);
    printf("  最大充电功率: %.2f kW\n", session->charger.maxPower);
    printf("  充电电压: %.2f V\n", session->charger.voltage);
    printf("  充电电流: %.2f A\n\n", session->charger.current);

    // 打印充电信息
    printf("充电信息:\n");
    printf("  全部充入电量: %.2f kWh\n", session->chargedEnergy);
    printf("  全部充电费用: %.2f 元\n", session->chargingCost);
    printf("  已充电百分比: %.2f%%\n", session->chargePercentage);

    // 打印充电开始时间
    char startTimeStr[26];
    ctime_r(&session->startTime, startTimeStr);
    printf("  充电开始时间: %s\n", startTimeStr);

    char useTimeBuffer_TempStr[100]; // 已经使用的时间 转换字符串
    formatTTaken(session->useTime, useTimeBuffer_TempStr, sizeof(useTimeBuffer_TempStr));
    printf("  一共用时: %s\n", useTimeBuffer_TempStr);
}
