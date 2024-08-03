#ifndef _CHARGING_H
#define _CHARGING_H

/**
 * @file name : charging.h
 * @brief     : charging充电站相关非通用函数
 * @author    : RISE_AND_GRIND@163.com
 * @date      : 2024/05/17
 * @version   : 1.0
 * @note      :
 *              默认800*480分辨率
 *              编译命令
 *                  arm-linux-gcc main.c ./src/* -o ./demo.out -I ./include -L ./lib -ljpeg -lfont  -lm   -pthread
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
#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "./font.h"
#include "./FileIOLibrary.h"
#include "./lcd.h"
#include "./touchpanel.h"
#include "./usermanagement.h"
#include "./charger.h"
#include <features.h>

/***************************************END******************************************/
// 服务次数
#define MIN_VALUE 198831
#define MAX_VALUE 998831
// 控制充电计算费用是按照实际时间s还是60倍加速(测试用)的开关 注意互斥, 不相等 1 0测试模式 0 1现实模式
#define TESTINGMODE 1 // 测试模式,加快60倍,时间要以分钟计算
#define PRACTICALMODE 0
/**************************************状态列表**************************************/
// 界面状态枚举
typedef enum
{
    state_SelectEntry = 0,              // 共享充电桩身份选择界面
    state_Authentication,               // 身份验证
    state_Login_Account,                // 登录界面-账号
    state_Login_Password,               // 登录界面-密码
    state_Registration_Account,         // 注册界面-账号
    state_Registration_Password,        // 注册界面-密码
    state_Registration_ConfirmPassword, // 注册界面-确认密码
    state_TermsInformation,             // 条款信息
    state_RegSuccess,                   // 注册成功界面
    state_SystemMain,                   // 系统主界面
    state_AccountEnquiry,               // 账户查询界面
    state_ChoosingChargingDev,          // 充电模式设备选择
    state_PriceList,                    // 充电价格展示
    state_PromptCharger,                // 提示:充电模式选择 – 请插好枪
    state_InCharge,                     // 充电中界面
    state_ChargingEndInfo,              // 充电结束信息
    state_PatternPayment,               // 付费方式选择窗口
    state_ScanPay,                      // 扫码支付
    state_NotSufficientFunds,           // 提示:余额不足
    state_RechargeInterface,            // 充值界面
    state_PaymentResult,                // 支付结果

} INTERFACE_STATE; // 系统界面状态枚举

// 登录注册界面状态枚举
typedef enum
{
    state_Ac_Blank = 0,       // 空白状态,没有选择
    state_Ac_Account,         // 选择输入账号
    state_Ac_Password,        // 选择输入密码
    state_Ac_ConfirmPassword, // 选择确认密码
    state_Ac_Login,           // 选择登录
    state_Ac_Register,        // 选择注册按钮

} ACCOUNTENTRY_STATE; // 系统界面状态枚举
// 键盘状态枚举
typedef enum
{
    state_KBDormancy = 0, // 键盘休眠
    state_KBActivate,     // 键盘激活

} KEYBOARD_STATE; // 系统界面状态枚举
/***************************************END******************************************/
/**************************************键盘读取处理**************************************/
#define MAX 100                     // 键盘输入栈最大储存字符数量
#define NOPREEMPTION usleep(200000) // 防止抢占 ,人的视觉暂停止是0.041666秒 usleep(41666)
typedef struct
{
    int top;         // 栈顶
    char items[MAX]; // 输入栈的大小
} Stack;
// 定义按钮结构体 用于存放一个有效按钮的坐标
typedef struct
{
    int x;      // 按钮左上角的 x 坐标
    int y;      // 按钮左上角的 y 坐标
    int width;  // 按钮宽度
    int height; // 按钮高度
} Button;
// 初始化键盘栈
void initStack(Stack *s);
// 检查键盘栈是否为空 1表示空 0不空
int isEmpty(Stack *s);
// 检查键盘栈是否满
int isFull(Stack *s);
// 键盘入栈操作
void push(Stack *s, char item);
// 键盘出栈操作
char pop(Stack *s);
// 清栈操作
void clearStack(Stack *s);
// 获取键盘栈的内容作为字符串
void getStackContent(Stack *s, char *str);
// 将输入坐标翻译为字符数字 并执行退格
char getNumberFromCoordinates(int x, int y, Stack *s);
char getNumberFromCoordinates2(int x, int y, Stack *s);

/***************************************END******************************************/
/**************************************线程任务**************************************/

/**
 * @name      displaytime_lcd
 * @brief     在LCD屏幕的固定位置显示时间, 每隔1秒刷新一次
 * @param     arg 线程例程的参数
 * @date      2024/05/18
 * @version   1.0 2024年5月18日10:23:17 基本功能实现
 * @note      ● 调用该函数之前，必须提前开线程！ 在main进入循环前开启线程
 *           pthread_t displaytime_lcd;
 *           pthread_create(&displaytime_lcd, NULL, displaytime_lcd, NULL);
 */
void *displaytime_lcd(void *arg);
/**
 * @name      ts_task
 * @brief     获取触摸屏按下的坐标
 * @param     arg 线程例程的参数
 * @date      2024/05/18
 * @version   1.0
 * @note      ● 调用该函数之前，必须提前开线程！ 在main进入循环前开启线程
 *           pthread_t ts_thread;
 *           pthread_create(&ts_thread, NULL, GetTouchVal_Once, NULL);
 */
void *ts_task(void *arg);
/***************************************END******************************************/
/**************************************状态处理函数声明**************************************/
void handle_SelectEntry();
void handle_Authentication();
void handle_Login_Account();
void handle_Login_Password();
void handle_Registration_Account();
void handle_Registration_Password();
void handle_Registration_ConfirmPassword();
void handle_TermsInformation();
void handle_RegSuccess();
void handle_SystemMain();
void handle_AccountEnquiry();
void handle_ChoosingChargingDev();
void handle_PriceList();
void handle_PromptCharger();
void handle_InCharge();
void handle_PromptStopCharging();
void handle_ChargingEndInfo();
void handle_PatternPayment();
void handle_ScanPay();
void handle_NotSufficientFunds();
void handle_RechargeInterface();
void handle_PaymentResult();
/***************************************END******************************************/
// 生成大于 x 的随机数
int generateRandomValue(int x);
// 生成大于等于 startnum 且小于等于 endnum 的随机整型数
int generateRandomInRange(int startnum, int endnum);
// 判断当前时间是否为高峰期的函数
bool isPeakTime();
// 获取一次本地系统时间, 并格式化输出  "YYYY年MM月DD日  HH:MM:SS"
/**
 * @name      format_time
 * @brief     获取一次本地系统时间, 并格式化输出  "YYYY年MM月DD日  HH:MM:SS"
 * @param     buffer 接收获取到并格式化后的字符串缓冲区
 * @param     buffer_size 缓冲区大小
 * @date      2024/05/17
 * @version   1.0
 * @note      ● 在程序结束之前，需要调用该函数对申请的LCD相关资源进行手动释放。
 *            ● 调用该函数之前，必须提前调用过lcd_init()，否则程序会段错误！！！
 */
void format_time(char *buffer, size_t buffer_size);
// 坐标清空为(-1,-1)
void CoordinateReset(void);
// 辅助函数：将秒数[seconds]转换为 "x小时x分钟x秒" 的格式 存放在[*buffer]中
void formatTimeTaken(int seconds, char *buffer, size_t bufferSize);
// 检查触控点是否在按钮范围内 按下返回1 没有按下返回0
/**
 * @name      isTouchButton
 * @brief     检查触控点是否在按钮范围内 按下返回1 没有按下返回0
 * @param     ts_x 触摸屏被按下的x坐标
 * @param     ts_y 触摸屏被按下的y坐标
 * @param     button  要判断的按钮的范围
 * @return
 *      @retval [1]触控有效 [0]触控无效
 * @date      2024/05/20
 * @version   1.0
 * @note
 */
int isTouchButton(int ts_x, int ts_y, Button button);
// 将用户充电时长timeTaken 转换为 "时分秒" 格式的字符串 ,例如: formatTTaken(log->timeTaken, timeTakenBuffer, sizeof(timeTakenBuffer));
void formatTTaken(int timeTaken, char *timeTakenResult, size_t size);
// 将用户消费日志时间consumption_at 转换为 "年月日 时分秒" 格式的字符串 例如formatConsumptionTime(log->consumption_at, timeBuffer, sizeof(timeBuffer));
void formatConsumptionTime(time_t consumption_at, char *consumptionResult, size_t size);

#endif