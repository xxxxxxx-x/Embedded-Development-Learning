#ifndef _USERMANAGEMENT_H
#define _USERMANAGEMENT_H
/**
 * @file name : usermanagement.c
 * @brief     : charging充电站用户管理相关功能
 * @author    : RISE_AND_GRIND@163.com
 * @date      : 2024/05/17
 * @version   : 1.0
 * @note      :
 *              默认800*480分辨率
 *              编译命令 arm-linux-gcc main.c ./src/* -o ./demo.out -I ./include -L ./lib -ljpeg -lfont  -lm   -pthread
 * CopyRight (c)  2023-2024   RISE_AND_GRIND@163.com   All Right Reseverd
 */
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
/***************************************END******************************************/
/***************************************预处理***************************************/
#define FILENAME "../data/UserInfoFile.txt" // 用户信息文件位置
// #define FILELOG "./data/"                  // 用户日志文件位置
#define REDPACKET (19.99) // 注册赠送的红包 元
/***************************************END******************************************/
/***************************************数据类型***************************************/
// 用户信息结构体
typedef struct
{
    char username[50];   // 用户名称
    char password[50];   // 用户密码
    char created_at[50]; // 用户创建时间
    float balance;       // 用户余额
} UserInfo;

// 用户消费信息结构体
typedef struct
{
    char username[50];            // 用户名称
    time_t consumption_at;        // 用户消费时间 (使用 time_t)
    float sum;                    // 消费金额
    float balance;                // 用户余额
    char chargingStationName[50]; // 充电站名称
    char model[50];               // 设备型号
    int timeTaken;                // 一次用时 (以秒为单位)
    float totalElectricity;       // 耗电量 (单位：度)
    char remark[50];              // 备注 例如充值或消费
} UserCashlog;
/***************************************END******************************************/

/**
 * @name      writeUserInfo
 * @brief     将用户信息写入用户信息文件中
 * @param     user 用户信息结构体
 * @param
 * @date      2024/05/19
 * @version   1.1
 * @note
 */
void writeUserInfo(UserInfo user);
/**
 * @name      readUserInfo
 * @brief     从用户信息文件中读取指定用户信息
 * @param     username 用户名称
 * @return
 *      @retval    [user]  成功读取到的用户信息结构体; [NULL] 用户不存在, 读取失败
 * @date      2024/05/18
 * @version   1.1
 * @note
 */
UserInfo *readUserInfo(const char *username);
/**
 * @name      updateUserInfo
 * @brief     更新用户信息到用户信息文件中
 * @param     user 用户信息结构体
 * @return
 *      @retval
 * @date      2024/05/18
 * @version   1.1
 * @note      会删除旧文件!! 并重命名为旧文件名称, --->新文件
 */
void updateUserInfo(UserInfo user);
/**
 * @name      registerUser
 * @brief     注册用户, 判断用户是否存在: 输入新用户信息, 然后写入到新文件中
 * @param     username 用户名(账号)
 * @param     password 用户密码
 * @return
 *      @retval [0]注册成功;  [1]用户已经存在;
 * @date      2024/05/19
 * @version   1.1
 * @note
 */
int registerUser(char *username, char *password);
/**
 * @name      loginUser
 * @brief     用户登录判断
 * @param     username 用户名(账号)
 * @param     password 用户密码
 * @return    [0]登录成功(账号密码正确);  [1]用户不存在;  [2]密码错误
 * @date      2024/05/18
 * @version   1.1
 * @note      支持中文
 */
int loginUser(char *username, char *password);
/***************************************用户消费日志处理******************************************/

// 增加用户消费日志
/**
 * @name      addUserCashlog
 * @brief     增加用户消费日志
 * @param     DestUeser_log 要写入的日志信息(内含用户名称)
 * @param
 * @return
 *      @retval
 * @date      2024/05/20
 * @version   1.0
 * @note
 */
void addUserCashlog(UserCashlog *DestUeser_log);
/**
 * @name      createLogFileName
 * @brief     创建日志文件名
 * @param     buffer 存放生成结果的缓存区
 * @param     bufferSize  缓存区大小
 * @param     username 用户名称
 * @return
 *      @retval
 * @date      2024/05/20
 * @version   1.0
 * @note
 */
void createLogFileName(char *buffer, size_t bufferSize, const char *username);
// 获取用户消费日志数量
/**
 * @name      getUserCashlogCount
 * @brief     获取用户消费日志数量, 去对应文件中查找
 * @param     username 用户名称
 * @return
 *      @retval [用户日志记录条数]
 * @date      2024/05/20
 * @version   1.0
 * @note
 */
int getUserCashlogCount(const char *username);
// 得到用户的第count条消费日志
/**
 * @name      queryUserCashlog
 * @brief     得到用户的第count条消费日志
 * @param     username 要查找的目标用户名称
 * @param     desCount 第count条消费日志 从0开始计算
 * @param     log      接收查找到的消费日志结构体信息
 * @return    0 成功，1 失败
 * @date      2024/05/20
 * @version   1.0
 * @note      暂时没有想到更好的方式, 一次性停顿逐行显示的话, 就需要中断或线程, 比较麻烦
 *            可以先调用计算函数计算有多少行
 */
int queryUserCashlog(const char *username, int desCount, UserCashlog *log);
// 在控制台上打印用户消费记录-->调试用
void printUserCashlog(UserCashlog *log);
#endif