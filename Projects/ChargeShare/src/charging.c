/**
 * @file name : charging.c
 * @brief     : charging充电站相关非通用函数
 * @author    : RISE_AND_GRIND@163.com
 * @date      : 2024/05/17
 * @version   : 1.0
 * @note      :
 *              默认800*480分辨率
 *              编译命令 arm-linux-gcc main.c ./src/LCD.c -o ./demo.out -I ./include -L ./lib -ljpeg
 * CopyRight (c)  2023-2024   RISE_AND_GRIND@163.com   All Right Reseverd
 */
#include "../include/charging.h"
/**************************************全局变量**************************************/
LcdDev *lcd;                      // LCD的硬件信息的地址
char time_str[50];                // 存放更新后的时间字符串
int ts_fd;                        // TS触摸屏的文件描述符
int ts_x = -1, ts_y = -1;         // 用于记录触摸屏的坐标, -1表示未按下
char Account_result[MAX];         // 存放账号临时输入结果
char Password_result[MAX];        // 存放密码临时输入结果
char confirmPassword_result[MAX]; // 存放再次输入密码临时输入结果

INTERFACE_STATE state = state_SelectEntry; // 状态机初始状态
bool refreshflag = true;                   // 各个状态图像刷新
bool backspaceflag = false;                // 是否按下退格 默认未按下 注意,使用完以后要恢复默认false
bool concealPassword_flag = false;         // 用户输入密码退格后是否未处理, false 处理完 true未处理 注意,使用完以后要恢复默认false
bool enterPasswordAgain_flag = false;      // 用于判断再次输入密码相关功能是否激活, 在输入密码后才会激活, 默认不激活false
UserInfo onlineUser;                       // 用于存放当前登入的用户信息
int NumberServices = 199931;               // 用于存放服务次数, 确保用户上电期间次数递增
/********充电相关*******/
char selectChargerResult[20]; // 储存充电枪的选择结果!, 用户后面的操作
CharSession chargingSession;  // 充电会话: 用于车辆信息的交换
// 充电枪型号参数: 型号  充电电压  充电电流 最大功率
Charger chargers[] = {
    {"1891JiAixi", 200.0, 31.82, 7.0},
    {"9240Aier", 750.0, 80.0, 60.0}}; // 充电枪信息表
int Payment_flag = -1;                // 支付界面的标志状态,用于表示是从哪里进来的 -1默认 0充电结算 1支付窗口(用户要充值)
/**********充值相关********/
float topupMoney = 0; // 存放要充值的金额
const char *CarsName[] = {
    "佩加西-曦武",
    "安斯-艾RH8",
    "贝克特-克格",
    "兰帕蒂-特力",
    "奥塞洛特-帕",
    "格罗提-图R",
    "皮斯特-811",
    "科尔-蒂克",
    "丁卡-杰斯特",
    "普根-利GTB",
};

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
void *displaytime_lcd(void *arg)
{
    // 调用死循环，并且不会退出
    while (1)
    {

        // 获取并格式化当前时间
        format_time(time_str, sizeof(time_str));
        if (state >= state_SystemMain)
        {
            displayText(lcd, FONT_PATH,
                        150, 20, getColor(255, 255, 145, 85),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), time_str,
                        345, 2);
        }

        sleep(1);
    }
}

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
void *ts_task(void *arg)
{
    // 调用死循环，并且不会退出
    while (1)
    {
        GetTouchVal_Once(ts_fd, &ts_x, &ts_y); // 获取触摸屏坐标
    }
}

/***************************************END******************************************/

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
void format_time(char *buffer, size_t buffer_size)
{
    time_t rawtime;
    struct tm *timeinfo;

    // 获取当前时间
    time(&rawtime);
    // 将时间转为本地时间
    timeinfo = localtime(&rawtime);

    // 格式化时间为 "YYYY年MM月DD日  HH:MM:SS"
    strftime(buffer, buffer_size, "%Y/%m/%d   %H:%M:%S", timeinfo);
}

/***************************************************界面状态处理函数定义**************************************************/
/***************************************身份选择界面******************************************/
/***************************************身份选择界面******************************************/
void handle_SelectEntry()
{

    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Select Entry共享充电桩选择界面\n");
    read_JPEG_file_LCD("../pic/SelectEntry.jpg", 0, 0, lcd->mp, 800);

    /************END***************/
    /************切换次态条件***************/
    while (1)
    {
        // 判断触控有效
        // 点击店家
        if ((ts_x >= 148 && ts_y >= 285) && (ts_x <= 148 + 127 && ts_y <= 285 + 127))
        {
            // state = state_Authentication; // 跳转身份验证界面
            break;
        }
        // 点击用户
        if ((ts_x >= 540 && ts_y >= 285) && (ts_x <= 540 + 127 && ts_y <= 285 + 127))
        {
            state = state_Authentication; // 跳转身份验证界面
            break;
        }
    }
    /************END***************/
    /*********初始化下一个状态环境**********/

    /************END***************/
}

/***************************************身份验证界面******************************************/
/***************************************身份验证界面******************************************/
void handle_Authentication()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling 身份校验选择界面\n");
    read_JPEG_file_LCD("../pic/Authentication.jpg", 0, 0, lcd->mp, 800);

    /************END***************/
    /************切换次态条件***************/
    while (1)
    {
        // 点击返回
        if ((ts_x >= 0 && ts_y >= 0) && (ts_x <= 0 + 19 && ts_y <= 0 + 19))
        {
            state = state_SelectEntry; // 返回身份选择界面
            break;
        }
        // 判断触控有效
        // 点击注册
        if ((ts_x >= 148 && ts_y >= 285) && (ts_x <= 148 + 127 && ts_y <= 285 + 127))
        {
            state = state_Registration_Account; // 跳转注册界面
            break;
        }
        // 点击登录
        if ((ts_x >= 540 && ts_y >= 285) && (ts_x <= 540 + 127 && ts_y <= 285 + 127))
        {
            state = state_Login_Account; // 跳转登录界面
            break;
        }
    }
    /************END***************/
    /*********初始化下一个状态环境**********/

    /************END***************/
}

/***************************************登录界面-请输入账号页******************************************/
/***************************************登录界面-请输入账号页******************************************/
void handle_Login_Account()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("进入登录界面-请输入账号页--->Handling Logind\n");
    read_JPEG_file_LCD("../pic/Login_Account.jpg", 0, 0, lcd->mp, 800);
    printf("登录界面-请输入账号页刷新完成\n");
    /************END***************/
    /************函数变量***************/
    /**初始化局部变量***/
    refreshflag = true;
    backspaceflag = false;

    Stack s;       // 创建键盘栈缓冲区 账号和密码公用 一次只能操作一种交互
    initStack(&s); // 初始化键盘栈

    char input; // 存放一次读取的一个字符
    int x, y;   // 存放当前坐标

    ACCOUNTENTRY_STATE state_selectAccount = state_Ac_Account; // 用户在该界面的交互枚举, 默认没有点击
    KEYBOARD_STATE state_keyboard = state_KBDormancy;          // 键盘休眠
    printf("登录界面-请输入账号页初始化完成\n");
    /************END***************/
    /************动作***************/
    printf("进入循环, 等待响应\n");
    while (1)
    {
        /************页面直接跳转条件***************/
        // 点击返回, 返回到共享充电桩身份选择界面
        if ((ts_x >= 333 && ts_y >= 63) && (ts_x <= 333 + 90 && ts_y <= 63 + 74))
        {
            printf("用户点击返回按钮, 返回身份验证界面\n");
            state = state_Authentication; // 返回到共享充电桩身份验证界面
            break;
        }
        /************END***************/
        /************判断用户是否有效点击输入***************/
        // 点击登录按钮
        if ((ts_x >= 243 && ts_y >= 338) && (ts_x <= 243 + 183 && ts_y <= 338 + 74))
        {
            printf("用户有效点击登录按钮\n");
            state_selectAccount = state_Ac_Login; // 执行登录操作
            break;
        }
        /************END***************/
        // 用户愿意在本页面输入账号密码尝试登录

        switch (state_selectAccount) // 用户输入交互小状态机
        {
        case state_Ac_Account: // 用户准备输入账号
        {
            printf("账号输入框已激活, 等待输入:\n");
            state_keyboard = state_KBActivate; // 键盘激活
            while (1)
            {
                int x_bk = ts_x;
                int y_bk = ts_y;
                if (state_keyboard == state_KBActivate)
                {
                    input = getNumberFromCoordinates(ts_x, ts_y, &s); // 获取临时输入一个字符
                    if (input != '\0' && s.top != 10)
                    {
                        printf("账号框输入了 %c\n", input);
                        push(&s, input);                     // 输入有效,压栈  这里放入显示
                        getStackContent(&s, Account_result); // 更新账号缓冲区
                        displayText(lcd, FONT_PATH,
                                    305, 55, getColor(255, 255, 145, 85),
                                    0, 3, 57, 0,
                                    getColor(255, 255, 255, 255), Account_result,
                                    462, 160); // 刷新显示账号
                    }
                }
                if (backspaceflag == true)
                {

                    getStackContent(&s, Account_result); // 更新账号缓冲区
                    displayText(lcd, FONT_PATH,
                                305, 55, getColor(255, 255, 145, 85),
                                0, 3, 57, 0,
                                getColor(255, 255, 255, 255), Account_result,
                                462, 160); // 退格刷新显示账号缓冲区
                    printf("账号输入过程中退格处理完成\n");
                    backspaceflag = false;
                }

                /*********状态切换条件**********/
                if ((x_bk >= 333 && y_bk >= 63) && (x_bk <= 333 + 90 && y_bk <= 63 + 74)) // 点击返回
                {
                    ts_x = -1;
                    ts_y = -1;
                    printf("用户在输入账号过程中点击返回按钮, 返回身份选择界面\n");
                    state = state_Authentication; // 返回到共享充电桩身份验证界面
                    return;
                }
                if ((x_bk >= 243 && y_bk >= 338) && (x_bk <= 243 + 183 && y_bk <= 338 + 74)) // 点击登录按钮
                {
                    ts_x = -1;
                    ts_y = -1;
                    printf("用户在输入账号过程中点击登录按钮, 执行登录操作\n");
                    state_selectAccount = state_Ac_Login;
                    break;
                }
                /*********END**********/
                ts_x = -1;
                ts_y = -1;
                NOPREEMPTION;
            }
            printf("已退出账号循环获取\n");
            /*********初始化下一个状态环境**********/
            printf("清空公用栈中\n");
            // 清空键盘临时栈
            for (; !isEmpty(&s);)
            {
                printf("清空账号栈ing, 为密码栈做准备\n");
                pop(&s);
            }
            printf("公用栈清空完成\n");
            ts_x = -1;
            ts_y = -1;
            state_keyboard = state_KBDormancy; // 键盘休眠
            printf("退出账号输入框\n");
            /************END***************/
        }
        break;
        case state_Ac_Login: // 用户点击登录
        {
            state = state_Login_Password; // 返回到共享充电桩身份验证界面
            return;
        }
        break;
        }
        /****************************************************************************************/
    }
    printf("退出登录界面-请输入账号页\n");
}

/***************************************登录界面-请输入密码页******************************************/
/***************************************登录界面-请输入密码页******************************************/
void handle_Login_Password()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("进入登录界面-请输入密码页--->Handling Logind\n");
    read_JPEG_file_LCD("../pic/Login_Password.jpg", 0, 0, lcd->mp, 800);
    printf("登录界面刷新完成\n");
    /************END***************/
    /************函数变量***************/
    /**初始化局部变量***/
    refreshflag = true;
    backspaceflag = false;
    concealPassword_flag = false;

    Stack s;                       // 创建键盘栈缓冲区 账号和密码公用 一次只能操作一种交互
    initStack(&s);                 // 初始化键盘栈
    Stack concealPassword_s;       // 创建密码隐藏缓冲区
    initStack(&concealPassword_s); // 初始密码隐藏化键盘栈

    char input;                       // 存放一次读取的一个字符
    int x, y;                         // 存放当前坐标
    char concealPassword_result[MAX]; // 存放隐藏密码临时输入结果

    ACCOUNTENTRY_STATE state_selectAccount = state_Ac_Password; // 用户在该界面的交互枚举, 默认没有点击
    KEYBOARD_STATE state_keyboard = state_KBDormancy;           // 键盘休眠
    printf("登录界面初始化完成\n");
    /************END***************/
    /************动作***************/
    printf("进入循环, 等待响应\n");
    while (1)
    {
        /************页面直接跳转条件***************/
        // 点击返回, 返回到共享充电桩身份选择界面
        if ((ts_x >= 333 && ts_y >= 63) && (ts_x <= 333 + 90 && ts_y <= 63 + 74))
        {
            printf("用户点击返回按钮, 返回身份选择界面\n");
            state = state_Authentication; // 返回到共享充电桩身份选择界面
            break;
        }

        /************END***************/
        /************判断用户是否有效点击输入***************/

        // 点击登录按钮
        if ((ts_x >= 243 && ts_y >= 338) && (ts_x <= 243 + 183 && ts_y <= 338 + 74))
        {
            printf("用户有效点击登录按钮\n");
            state_selectAccount = state_Ac_Login; // 执行登录操作
        }
        /************END***************/
        // 用户愿意在本页面输入账号密码尝试登录
        switch (state_selectAccount) // 用户输入交互小状态机
        {
        case state_Ac_Password: // 用户准备输入密码
        {
            printf("密码输入框已激活, 等待输入:\n");
            state_keyboard = state_KBActivate; // 键盘激活
            while (1)
            {
                int x_bbk = ts_x;
                int y_bbk = ts_y;
                if (state_keyboard == state_KBActivate)
                {
                    input = getNumberFromCoordinates(ts_x, ts_y, &s); // 获取临时输入一个字符
                    if (concealPassword_flag == true)                 // 如果产生了密码退格的事项
                    {
                        printf("用户在输入密码过程中 点击退格, 隐藏密码处理:\n");
                        pop(&concealPassword_s);      // 若之前用户在账户输入过程中退格, 会导致这里会自动清空一次, 会报栈空, 不影响
                        concealPassword_flag = false; // 处理完成
                    }

                    if (input != '\0' && s.top != 13)
                    {
                        printf("密码框输入了: %c\n", input);
                        push(&s, input);                                             // 输入有效,压栈  这里放入显示
                        push(&concealPassword_s, '*');                               // 同时更新隐藏密码栈
                        getStackContent(&s, Password_result);                        // 更新密码缓冲区
                        getStackContent(&concealPassword_s, concealPassword_result); // 更新隐藏密码缓冲区
                        displayText(lcd, FONT_PATH,
                                    305, 55, getColor(255, 255, 145, 85),
                                    0, 3, 57, 0,
                                    getColor(255, 255, 255, 255), concealPassword_result,
                                    462, 160); // 刷新显示账号
                    }
                }
                if (backspaceflag == true)
                {

                    getStackContent(&s, Password_result); // 更新密码缓冲区
                    if (isEmpty(&s))
                    {
                        for (; !isEmpty(&concealPassword_s);)
                        {
                            pop(&concealPassword_s);
                        }
                    }
                    getStackContent(&concealPassword_s, concealPassword_result); // 更新隐藏密码缓冲区
                    displayText(lcd, FONT_PATH,
                                305, 55, getColor(255, 255, 145, 85),
                                0, 3, 57, 0,
                                getColor(255, 255, 255, 255), concealPassword_result,
                                462, 160); // 退格刷新显示密码缓冲区
                    printf("用户在输入密码过程中 点击退格, 密码处理完成\n");
                    backspaceflag = false;
                }
                /*********状态切换条件**********/
                if ((x_bbk >= 333 && y_bbk >= 63) && (x_bbk <= 333 + 90 && y_bbk <= 63 + 74)) // 点击返回
                {
                    state = state_Authentication; // 返回到共享充电桩身份选择界面
                    printf("在输入密码过程中点击返回按钮, 返回身份选择界面\n");
                    return;
                }
                if ((x_bbk >= 243 && y_bbk >= 338) && (x_bbk <= 243 + 183 && y_bbk <= 338 + 74)) // 点击登录按钮
                {
                    printf("在输入密码过程中点击登录按钮\n");
                    state_selectAccount = state_Ac_Login;
                    break;
                }
                /*********END**********/
                NOPREEMPTION;
            }
            ts_x = -1;
            ts_y = -1;
            NOPREEMPTION;
            printf("已退出密码循环获取\n");
            /*********初始化下一个状态环境**********/
            // 清空键盘临时栈
            printf("清空公用栈中\n");
            for (; !isEmpty(&s);)
            {
                printf("清空密码栈ing\n");
                pop(&s);
            }
            printf("公用栈清空完成\n");
            // state_keyboard = state_KBDormancy; // 键盘休眠
            /************END***************/
            printf("退出密码输入框\n");
        }
        break;
        case state_Ac_Login: // 用户点击登录
        {
            int LoginResult = -1; // 存放登录结果
            printf("调用登录模块, 进入登录事件处理ing\n");
            printf("你输入的账号%s 密码%s \n", Account_result, Password_result);
            LoginResult = loginUser(Account_result, Password_result);
            switch (LoginResult)
            {
            case -1:
            {
                printf("登录事件-->处理异常!!\n");
            }
            break;
            case 0:
            {
                printf("登录事件-->登录成功, 即将进入主界面\n");
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "登录成功",
                            462, 286);                        // 显示提示登录成功
                usleep(500000);                               // 提示一秒
                state = state_SystemMain;                     // 次态切换-->主界面
                strcpy(onlineUser.username, Account_result);  // 更新当前登录账户的信息
                strcpy(onlineUser.password, Password_result); // 更新当前登录账户的信息
                return;
            }
            break;
            case 1:
            {
                printf("登录事件-->用户不存在, 跳转注册页面 \n");
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 38, 0,
                            getColor(0, 255, 255, 255), "用户不存在,请注册!",
                            462, 290);              // 显示提示用户不存在
                usleep(1500000);                    // 提示一秒
                state = state_Registration_Account; // 次态切换-->注册界面
                return;
            }
            break;
            case 2:
            {
                printf("登录事件-->密码错误, 返回身份选择界面\n");
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "验证不通过",
                            462, 286);       // 显示提示密码错误
                usleep(1500000);             // 提示一秒
                state = state_Login_Account; // 次态切换-->身份选择
                return;
            }
            break;
            }
            CoordinateReset();
            printf("调试-->登录处理功能异常!!\n");
        }
        break;
        }
        /****************************************************************************************/
    }
    printf("退出登录界面\n");
}

/***************************************注册界面******************************************/
/***************************************注册界面******************************************/
void handle_Registration_Account()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("进入注册界面--->Handling Registration\n");
    read_JPEG_file_LCD("../pic/Registration_Account.jpg", 0, 0, lcd->mp, 800);
    printf("注册界面刷新完成\n");
    /************END***************/
    /************函数变量***************/
    /**初始化局部变量***/
    // 定义合法的前3位手机号前缀
    const char *validPrefixes[] = {
        "133", "135", "137", "138", "139", "149", "153", "173", "177", "180", "181", "189",
        "190", "191", "193", "199"};
    // 计算合法前缀的数量
    const int numValidPrefixes = sizeof(validPrefixes) / sizeof(validPrefixes[0]);
    refreshflag = true;
    backspaceflag = false;
    concealPassword_flag = false;

    Stack s;       // 键盘栈缓冲区 账号和密码公用 一次只能操作一种交互
    initStack(&s); // 初始化键盘栈

    char input; // 存放一次读取的一个字符
    int x, y;   // 存放当前坐标

    ACCOUNTENTRY_STATE state_selectAccount = state_Ac_Account; // 用户在该界面的交互枚举, 默认没有点击
    KEYBOARD_STATE state_keyboard = state_KBDormancy;          // 键盘休眠
    printf("注册界面初始化完成\n");
    /************END***************/
    /************动作***************/
    printf("进入注册循环, 等待响应\n");

    while (1)
    {
        /************页面直接跳转条件***************/
        // 用户点击返回, 可能是想起账户信息/密码了, 返回登录界面
        if ((ts_x >= 333 && ts_y >= 63) && (ts_x <= 333 + 90 && ts_y <= 63 + 74))
        {
            printf("用户点击返回按钮, 返回身份选择界面\n");
            state = state_Authentication; // 返回到登录界面
            break;
        }
        /************END***************/
        /************判断用户是否有效点击输入***************/

        // 点击确认注册按钮
        if ((ts_x >= 243 && ts_y >= 338) && (ts_x <= 243 + 183 && ts_y <= 338 + 74))
        {
            printf("用户有效点击注册按钮\n");
            state_selectAccount = state_Ac_Register; // 执行注册操作
        }
        /************END***************/
        // 用户愿意在本页面注册
        switch (state_selectAccount) // 用户输入交互小状态机
        {
        case state_Ac_Account: // 用户准备输入账号
        {
            printf("账号输入框已激活, 等待输入:\n");
            state_keyboard = state_KBActivate; // 键盘激活
            while (1)
            {
                int x_bk = ts_x;
                int y_bk = ts_y;
                if (state_keyboard == state_KBActivate)
                {
                    input = getNumberFromCoordinates(ts_x, ts_y, &s); // 获取临时输入一个字符
                    if (input != '\0' && s.top != 10)
                    {
                        printf("账号框输入了 %c\n", input);
                        push(&s, input);                     // 输入有效,压栈  这里放入显示
                        getStackContent(&s, Account_result); // 更新账号缓冲区
                        displayText(lcd, FONT_PATH,
                                    305, 55, getColor(255, 255, 145, 85),
                                    0, 3, 57, 0,
                                    getColor(255, 255, 255, 255), Account_result,
                                    462, 160); // 刷新显示账号
                    }
                }
                if (backspaceflag == true)
                {

                    getStackContent(&s, Account_result); // 更新账号缓冲区
                    displayText(lcd, FONT_PATH,
                                305, 55, getColor(255, 255, 145, 85),
                                0, 3, 57, 0,
                                getColor(255, 255, 255, 255), Account_result,
                                462, 160); // 刷新显示账号
                    printf("账号输入过程中退格处理完成\n");
                    backspaceflag = false;
                }
                /*********状态切换条件**********/
                if ((x_bk >= 333 && y_bk >= 63) && (x_bk <= 333 + 90 && y_bk <= 63 + 74))
                {
                    ts_x = -1;
                    ts_y = -1;
                    printf("用户在输入账号过程中点击返回按钮, 返回登录界面\n");
                    state = state_Authentication; // 返回到登录界面
                    return;
                }
                if ((x_bk >= 243 && y_bk >= 338) && (x_bk <= 243 + 183 && y_bk <= 338 + 74))
                {
                    ts_x = -1;
                    ts_y = -1;
                    printf("用户在输入账号过程中点击确认注册按钮, 执行注册操作\n");
                    state_selectAccount = state_Ac_Register;
                    break;
                }
                /*确认密码框在此无效,不激活*/
                /*********END**********/
                ts_x = -1;
                ts_y = -1;
                NOPREEMPTION;
            }
            printf("已退出账号循环获取\n");
            /*********初始化下一个状态环境**********/
            printf("清空公用栈中\n");
            // 清空键盘临时栈
            for (; !isEmpty(&s);)
            {
                printf("清空账号栈ing, 为密码栈做准备\n");
                pop(&s);
            }
            printf("公用栈清空完成\n");
            ts_x = -1;
            ts_y = -1;
            state_keyboard = state_KBDormancy; // 键盘休眠
            printf("退出账号输入框\n");
            /************END***************/
        }
        break;
        case state_Ac_Register: // 用户点击注册
        {
            int RegisterResult = -1;  // 存放注册结果
            int PhonevalidResult = 0; // 存放输入手机号合法判断 0不合法 1合法 初始值0
            printf("调用注册模块, 进入注册事件处理ing\n");

            // 判断是否合法
            if (strlen(Account_result) != 11) // 手机号长度不足3位或超过6位，长度不合法
            {
                PhonevalidResult = 0;
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "账号应该是11位的手机号",
                            462, 160);
                usleep(1500000); // 提示一秒
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "请重新输入",
                            462, 160);              // 刷新显示账号               // 显示提示两次密码不相等
                usleep(1500000);                    // 提示一秒
                state = state_Registration_Account; // 次态切换-->重新加载注册界面
                return;
            }

            /***** 手机号内容检测 ******/
            // 提取手机号的前3位
            char prefix[4];
            strncpy(prefix, Account_result, 3);
            prefix[3] = '\0'; // 确保前缀字符串以'\0'结尾

            // 检查前3位是否在合法前缀列表中
            for (int i = 0; i < numValidPrefixes; i++)
            {
                if (strcmp(prefix, validPrefixes[i]) == 0) // 逐一判断
                {
                    PhonevalidResult = 1; // 合法前缀
                }
                //   printf("调试prefix:%s,validPrefixes:%s\n", prefix, validPrefixes[i]);
            }

            // 不合法处理
            if (PhonevalidResult == 0)
            {
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "账号非法!",
                            462, 160);
                usleep(1500000);                    // 提示0.2秒
                state = state_Registration_Account; // 次态切换-->重新加载注册界面
                return;
            }
            else if (PhonevalidResult == 1) // 合法账号
            {
                state = state_Registration_Password;
                return;
            }
            CoordinateReset();
            printf("调试-->注册处理功能异常!!\n");
        }
        break;
        }
        /****************************************************************************************/
    }
    printf("退出注册界面\n");
}

/***************************************注册界面******************************************/
/***************************************注册界面******************************************/
void handle_Registration_Password()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("进入注册界面--->Handling Registration\n");
    read_JPEG_file_LCD("../pic/Registration_Password.jpg", 0, 0, lcd->mp, 800);
    printf("注册界面刷新完成\n");
    /************END***************/
    refreshflag = true;
    backspaceflag = false;
    concealPassword_flag = false;

    Stack s;                       // 键盘栈缓冲区 账号和密码公用 一次只能操作一种交互
    initStack(&s);                 // 初始化键盘栈
    Stack concealPassword_s;       // 密码隐藏缓冲区
    initStack(&concealPassword_s); // 初始密码隐藏化键盘栈

    char input; // 存放一次读取的一个字符
    int x, y;   // 存放当前坐标

    char concealPassword_result[MAX];                           // 存放隐藏密码临时输入结果
    ACCOUNTENTRY_STATE state_selectAccount = state_Ac_Password; // 用户在该界面的交互枚举, 默认没有点击
    KEYBOARD_STATE state_keyboard = state_KBDormancy;           // 键盘休眠
    printf("注册界面初始化完成\n");
    /************END***************/
    /************动作***************/
    printf("进入注册循环, 等待响应\n");

    while (1)
    {
        /************页面直接跳转条件***************/
        // 用户点击返回, 可能是想起账户信息/密码了, 返回登录界面
        if ((ts_x >= 333 && ts_y >= 63) && (ts_x <= 333 + 90 && ts_y <= 63 + 74))
        {
            printf("用户点击返回按钮, 返回身份选择界面\n");
            state = state_Authentication; // 返回到登录界面
            break;
        }

        /************END***************/

        // 点击确认注册按钮
        if ((ts_x >= 243 && ts_y >= 338) && (ts_x <= 243 + 183 && ts_y <= 338 + 74))
        {
            printf("用户有效点击注册按钮\n");
            state_selectAccount = state_Ac_Register; // 执行注册操作
        }

        /************END***************/
        // 用户愿意在本页面注册
        switch (state_selectAccount) // 用户输入交互小状态机
        {
        case state_Ac_Password: // 用户准备输入密码
        {
            printf("密码输入框已激活, 等待输入:\n");
            state_keyboard = state_KBActivate; // 键盘激活
            while (1)
            {
                int x_bbk = ts_x;
                int y_bbk = ts_y;
                if (state_keyboard == state_KBActivate)
                {
                    input = getNumberFromCoordinates(ts_x, ts_y, &s); // 获取临时输入一个字符
                    if (concealPassword_flag == true)                 // 如果产生了密码退格的事项
                    {
                        printf("用户在输入密码过程中 点击退格, 隐藏密码处理:\n");
                        pop(&concealPassword_s);      // 若之前用户在账户输入过程中退格, 会导致这里会自动清空一次, 会报栈空, 不影响
                        concealPassword_flag = false; // 处理完成
                    }

                    if (input != '\0' && s.top != 13)
                    {
                        printf("密码框输入了: %c\n", input);
                        push(&s, input);                                             // 输入有效,压栈  这里放入显示
                        push(&concealPassword_s, '*');                               // 同时更新隐藏密码栈
                        getStackContent(&s, Password_result);                        // 更新密码缓冲区
                        getStackContent(&concealPassword_s, concealPassword_result); // 更新隐藏密码缓冲区
                        displayText(lcd, FONT_PATH,
                                    305, 55, getColor(255, 255, 145, 85),
                                    0, 3, 48, 0,
                                    getColor(0, 255, 255, 255), concealPassword_result,
                                    462, 199);
                    }
                }
                if (backspaceflag == true)
                {
                    getStackContent(&s, Password_result);
                    if (isEmpty(&s))
                    {
                        for (; !isEmpty(&concealPassword_s);)
                        {
                            pop(&concealPassword_s);
                        }
                    } // 更新密码缓冲区
                    getStackContent(&concealPassword_s, concealPassword_result); // 更新隐藏密码缓冲区
                    displayText(lcd, FONT_PATH,
                                305, 55, getColor(255, 255, 145, 85),
                                0, 3, 48, 0,
                                getColor(0, 255, 255, 255), concealPassword_result,
                                462, 199);
                    printf("用户在输入密码过程中 点击退格, 密码处理完成\n");
                    backspaceflag = false;
                }
                /*********状态切换条件**********/
                if ((x_bbk >= 333 && y_bbk >= 63) && (x_bbk <= 333 + 90 && y_bbk <= 63 + 74))
                {
                    state = state_Authentication; // 返回到登录
                    printf("在输入密码过程中点击返回按钮, 返回登录界面\n");
                    return;
                }

                if ((x_bbk >= 243 && y_bbk >= 338) && (x_bbk <= 243 + 183 && y_bbk <= 338 + 74)) // 点击确认注册按钮
                {
                    printf("在输入密码过程中点击确认注册按钮\n");
                    state_selectAccount = state_Ac_Register;
                    break;
                }

                /*********END**********/
                NOPREEMPTION;
            }
            ts_x = -1;
            ts_y = -1;
            NOPREEMPTION;
            printf("已退出密码循环获取\n");
            /*********初始化下一个状态环境**********/
            // 清空键盘临时栈
            printf("清空公用栈中\n");
            for (; !isEmpty(&s);)
            {
                printf("清空密码栈ing\n");
                pop(&s);
            }
            printf("公用栈清空完成\n");
            state_keyboard = state_KBDormancy; // 键盘休眠
            /************END***************/
            printf("退出密码输入框\n");
        }
        break;
        case state_Ac_Register: // 用户点击注册
        {
            state = state_Registration_ConfirmPassword; // 次态切换-->登录界面
            return;
        }
            /****************************************************************************************/
        }
    }
    printf("退出注册界面\n");
}

/***************************************注册界面******************************************/
/***************************************注册界面******************************************/

void handle_Registration_ConfirmPassword()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("进入注册界面--->Handling Registration\n");
    read_JPEG_file_LCD("../pic/Registration_ConfirmPassword.jpg", 0, 0, lcd->mp, 800);
    printf("注册界面刷新完成\n");
    /************END***************/
    /************函数变量***************/
    /**初始化局部变量***/
    // 定义合法的前3位手机号前缀
    const char *validPrefixes[] = {
        "133", "149", "153", "173", "177", "180", "181", "189",
        "190", "191", "193", "199"};
    // 计算合法前缀的数量
    const int numValidPrefixes = sizeof(validPrefixes) / sizeof(validPrefixes[0]);
    refreshflag = true;
    backspaceflag = false;
    concealPassword_flag = false;

    Stack s;                       // 键盘栈缓冲区 账号和密码公用 一次只能操作一种交互
    initStack(&s);                 // 初始化键盘栈
    Stack concealPassword_s;       // 密码隐藏缓冲区
    initStack(&concealPassword_s); // 初始密码隐藏化键盘栈

    char input; // 存放一次读取的一个字符
    int x, y;   // 存放当前坐标

    char confirmconcealPwd_result[MAX];                                // 存放隐藏再次输入密码临时输入结果
    ACCOUNTENTRY_STATE state_selectAccount = state_Ac_ConfirmPassword; // 用户在该界面的交互枚举, 默认没有点击
    KEYBOARD_STATE state_keyboard = state_KBDormancy;                  // 键盘休眠
    printf("注册界面初始化完成\n");
    /************END***************/
    /************动作***************/
    printf("进入注册循环, 等待响应\n");

    while (1)
    {
        /************页面直接跳转条件***************/
        // 用户点击返回, 可能是想起账户信息/密码了, 返回登录界面
        if ((ts_x >= 333 && ts_y >= 63) && (ts_x <= 333 + 90 && ts_y <= 63 + 74))
        {
            printf("用户点击返回按钮, 返回身份选择界面\n");
            state = state_Authentication; // 返回到登录界面
            break;
        }

        /************判断用户是否有效点击输入***************/
        // 点击确认注册按钮
        if ((ts_x >= 243 && ts_y >= 338) && (ts_x <= 243 + 183 && ts_y <= 338 + 74))
        {
            printf("用户有效点击注册按钮\n");
            state_selectAccount = state_Ac_Register; // 执行注册操作
        }
        /************END***************/
        // 用户愿意在本页面注册
        switch (state_selectAccount) // 用户输入交互小状态机
        {
        case state_Ac_ConfirmPassword: // 用户准备再次输入密码
        {
            printf("再次输入密码框已激活, 等待输入:\n");
            state_keyboard = state_KBActivate; // 键盘激活
            while (1)
            {
                int x_bbk = ts_x;
                int y_bbk = ts_y;
                if (state_keyboard == state_KBActivate)
                {
                    input = getNumberFromCoordinates(ts_x, ts_y, &s); // 获取临时输入一个字符
                    if (concealPassword_flag == true)                 // 如果产生了密码退格的事项
                    {
                        printf("用户在输入密码过程中 点击退格, 隐藏密码处理:\n");
                        pop(&concealPassword_s);      // 若之前用户在账户输入过程中退格, 会导致这里会自动清空一次, 会报栈空, 不影响
                        concealPassword_flag = false; // 处理完成
                    }

                    if (input != '\0' && s.top != 13)
                    {
                        printf("密码框输入了: %c\n", input);
                        push(&s, input);                                               // 输入有效,压栈  这里放入显示
                        push(&concealPassword_s, '*');                                 // 同时更新隐藏密码栈
                        getStackContent(&s, confirmPassword_result);                   // 更新密码缓冲区
                        getStackContent(&concealPassword_s, confirmconcealPwd_result); // 更新隐藏密码缓冲区
                        displayText(lcd, FONT_PATH,
                                    305, 55, getColor(255, 255, 145, 85),
                                    0, 3, 57, 0,
                                    getColor(255, 255, 255, 255), confirmconcealPwd_result,
                                    462, 160); // 刷新显示账号
                    }
                }
                if (backspaceflag == true)
                {

                    getStackContent(&s, confirmPassword_result); // 更新密码缓冲区
                    if (isEmpty(&s))
                    {
                        for (; !isEmpty(&concealPassword_s);)
                        {
                            pop(&concealPassword_s);
                        }
                    }
                    getStackContent(&concealPassword_s, confirmconcealPwd_result); // 更新隐藏密码缓冲区
                    displayText(lcd, FONT_PATH,
                                305, 55, getColor(255, 255, 145, 85),
                                0, 3, 57, 0,
                                getColor(255, 255, 255, 255), confirmconcealPwd_result,
                                462, 160); // 刷新显示账号
                    printf("用户在输入密码过程中 点击退格, 密码处理完成\n");
                    backspaceflag = false;
                }
                /*********状态切换条件**********/
                if ((x_bbk >= 333 && y_bbk >= 63) && (x_bbk <= 333 + 90 && y_bbk <= 63 + 74))
                {
                    state = state_Authentication; // 返回到登录
                    printf("在输入密码过程中点击返回按钮, 返回登录界面\n");
                    return;
                }
                if ((x_bbk >= 243 && y_bbk >= 338) && (x_bbk <= 243 + 183 && y_bbk <= 338 + 74))
                {
                    printf("在输入密码过程中点击确认注册按钮\n");
                    state_selectAccount = state_Ac_Register;
                    break;
                }
                /*********END**********/
                NOPREEMPTION;
            }
            ts_x = -1;
            ts_y = -1;
            NOPREEMPTION;
            printf("已退出再次输入密码循环获取\n");
            /*********初始化下一个状态环境**********/
            // 清空键盘临时栈
            printf("清空公用栈中\n");
            for (; !isEmpty(&s);)
            {
                printf("清空密码栈ing\n");
                pop(&s);
            }
            printf("公用栈清空完成\n");
            state_keyboard = state_KBDormancy; // 键盘休眠
            /************END***************/
            printf("退出密码再次输入框\n");
        }
        break;
        case state_Ac_Register: // 用户点击注册
        {
            int RegisterResult = -1;  // 存放注册结果
            int PhonevalidResult = 0; // 存放输入手机号合法判断 0不合法 1合法 初始值0
            printf("调用注册模块, 进入注册事件处理ing\n");
            printf("你输入的账号: %s\n密码: %s\n再次输入的密码: %s\n", Account_result, Password_result, confirmPassword_result);
            if (strcmp(Password_result, confirmPassword_result) != 0) // 如果不相等,  相等为1
            {
                printf("注册事件-->两次密码不相等\n");
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "两次密码不相等",
                            462, 286);
                usleep(1500000); // 提示0.8秒
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "请重新输入",
                            462, 286);
                usleep(500000);                      // 提示一秒
                state = state_Registration_Password; // 次态切换-->重新加载注册界面
                return;
            }
            // 两次密码相等
            // 判断是否合法
            if (strlen(Account_result) != 11)
            {
                PhonevalidResult = 0;
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "账号应该是11位的手机号",
                            462, 286);
                usleep(1500000); // 提示0.8秒
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "请重新输入",
                            462, 286);
                usleep(1500000);                    // 提示一秒
                state = state_Registration_Account; // 次态切换-->重新加载注册界面
                return;
            }

            /***** 手机号内容检测 ******/
            // 提取手机号的前3位
            char prefix[4];
            strncpy(prefix, Account_result, 3);
            prefix[3] = '\0'; // 确保前缀字符串以'\0'结尾

            // 检查前3位是否在合法前缀列表中
            for (int i = 0; i < numValidPrefixes; i++)
            {
                if (strcmp(prefix, validPrefixes[i]) == 0) // 逐一判断
                {
                    PhonevalidResult = 1; // 合法前缀
                }
                //   printf("调试prefix:%s,validPrefixes:%s\n", prefix, validPrefixes[i]);
            }

            // 不合法处理
            if (PhonevalidResult == 0)
            {
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "账号非法!",
                            462, 286);
                usleep(1500000); // 提示0.2秒
                displayText(lcd, FONT_PATH,
                            305, 55, getColor(0, 113, 113, 241),
                            0, 2, 48, 0,
                            getColor(0, 255, 255, 255), "请重新输入",
                            462, 286);
                usleep(1500000);                    // 提示一秒
                state = state_Registration_Account; // 次态切换-->重新加载注册界面
                return;
            }
            else if (PhonevalidResult == 1) // 合法账号
            {
                RegisterResult = registerUser(Account_result, Password_result);
                switch (RegisterResult)
                {
                case -1:
                {
                    printf("注册事件-->处理异常!!\n");
                }
                break;
                case 0:
                {
                    printf("注册事件-->注册成功, 进入恭喜注册界面\n");
                    displayText(lcd, FONT_PATH,
                                305, 55, getColor(0, 113, 113, 241),
                                0, 2, 48, 0,
                                getColor(0, 255, 255, 255), "注册成功",
                                462, 286);
                    usleep(1500000);                              // 提示0.8秒
                    state = state_RegSuccess;                     // 次态切换-->登录界面
                    strcpy(onlineUser.username, Account_result);  // 更新当前登录账户的信息
                    strcpy(onlineUser.password, Password_result); // 更新当前登录账户的信息
                    return;
                }
                break;
                case 1:
                {
                    printf("注册事件-->用户已经存在 \n");
                    displayText(lcd, FONT_PATH,
                                305, 55, getColor(0, 113, 113, 241),
                                0, 2, 48, 0,
                                getColor(0, 255, 255, 255), "用户已存在,请登录",
                                462, 286);
                    usleep(1500000);             // 提示一秒
                    state = state_Login_Account; // 次态切换-->注册界面
                    return;
                }
                break;
                }
            }
            CoordinateReset();
            printf("调试-->注册处理功能异常!!\n");
        }
        break;
        }
        /****************************************************************************************/
    }
    printf("退出注册界面\n");
}

/***************************************注册成功界面******************************************/
/***************************************注册成功界面******************************************/
void handle_RegSuccess()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Registration Success注册成功界面\n");
    read_JPEG_file_LCD("../pic/RegSuccess.jpg", 0, 0, lcd->mp, 800);
    printf("注册成功界面刷新完成\n");

    char messageID[100];         // 用于存放拼接后的用户ID信息
    char message[100];           // 用于存放拼接后的红包消息
    float redPacket = REDPACKET; // 将宏定义的红包值赋给变量

    // float redPacket = REDPACKET; // 将宏定义的红包值赋给变量
    /*显示当前账户和获赠金额*/
    // 当前用户显示
    // 使用 sprintf 函数进行字符串拼接
    // 赠送金额显示
    sprintf(message, "恭喜您，抽中专享红包，赠送您%.2f元", redPacket);
    // 输出拼接后的消息
    printf("显示恭喜信息:\n%s\n", message);
    displayText(lcd, FONT_PATH,
                600, 55, getColor(0, 70, 62, 255),
                50, 11, 38, 0,
                getColor(0, 255, 255, 255), message,
                100, 278); // 显示红包信息
    /************切换次态条件***************/
    while (1)
    {
        // 点击开始使用
        if ((ts_x >= 100 && ts_y >= 284) && (ts_x <= 100 + 600 && ts_y <= 284 + 55))
        {
            state = state_SystemMain; //
            printf("开始使用, 进入主界面 \n");
            break;
        }
    }
    printf("离开注册成功界面\n");
    /************END***************/
}

/***************************************系统主界面******************************************/
/***************************************系统主界面******************************************/
void handle_SystemMain()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling System Main系统主界面\n");
    printf("系统主界面刷新完成\n");
    read_JPEG_file_LCD("../pic/SystemMain.jpg", 0, 0, lcd->mp, 800);
    Payment_flag = 1; // 复位扣费相关模式.防止有用户冲完一次后又返回去充值
    topupMoney = 0;   // 充值金额恢复默认
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    while (1)
    {
        // 点击返回
        if ((ts_x >= 0 && ts_y >= 0) && (ts_x <= 0 + 30 && ts_y <= 0 + 20))
        {
            state = state_Authentication; // 返回身份选择界面
            printf("用户从主菜单退出登录, 跳转身份选择界面 \n");
            // 清空在线用户信息
            memset(onlineUser.username, 0, sizeof(onlineUser.username));
            break;
        }
        // 点击开始充电
        if ((ts_x >= 83 && ts_y >= 70) && (ts_x <= 83 + 150 && ts_y <= 70 + 346))
        {
            state = state_ChoosingChargingDev; // 进入充电模式设备选择界面
            printf("用户点击开始充电, 进入充电模式设备选择界面 \n");
            break;
        }
        // 点击充值缴费
        if ((ts_x >= 320 && ts_y >= 70) && (ts_x <= 320 + 150 && ts_y <= 70 + 346))
        {
            state = state_RechargeInterface; // 进入充值界面
            printf("用户点击充值缴费, 进入充值 界面 \n");
            break;
        }
        // 点击账户查询
        if ((ts_x >= 556 && ts_y >= 70) && (ts_x <= 556 + 150 && ts_y <= 70 + 346))
        {
            state = state_AccountEnquiry; // 进入账户查询界面
            printf("用户点击账户查询, 进入账户查询界面 \n");
            break;
        }
    }
    printf("离开系统主界面\n");
    /************END***************/
}

/***************************************账户查询界面******************************************/
/***************************************账户查询界面******************************************/
void handle_AccountEnquiry()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Account Enquiry账户查询界面\n");
    printf("账户查询界面刷新完成\n");
    read_JPEG_file_LCD("../pic/AccountEnquiry.jpg", 0, 0, lcd->mp, 800);
    // 装载按钮位置
    Button getback_Button = {0, 0, 30, 20};       // 返回按钮 x y 宽 高
    Button previous_Button = {381, 387, 162, 51}; // 上一页
    Button next_Button = {577, 389, 162, 51};     // 下一页
    // 局部变量初始化
    int userLogCount = 0; // 日志翻页计数 初始化为0
    UserCashlog log;      // 用户日志的缓存区
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4);      // 显示当前登录用户名称
    char userLogCountStr[20]; // 存储转换计数后的字符串
    //// strcpy(log.username, onlineUser.username); // 装载用户名称
    printf("开始装载用户:%s消费信息\n", onlineUser.username);
    int userLognum = getUserCashlogCount(onlineUser.username); // 用户日志数量
    char timeBuffer[50];                                       // 存放用户消费时间转换结果
    char timeTakenBuffer[50];                                  // 存放充电用时转换结果
    char totalElectricityBuffer[50];                           // 存放用户充入电量字符串转换结果
    char sumBuffer[50];                                        // 存放用户消费金额字符串转换结果
    char balanceBuffer[50];                                    // 存放日志用户余额字符串转换结果
    char NowBalanceBuffer[50];                                 // 存放在线用户余额字符串转换结果
    bool refresh_falg = false;                                 // 控制日志刷新, 防止速度过快导致字体文件打开错误 默认关闭false 打开true
    int queryUserCashlogResult;
    /*******************在线用户信息刷新***********************/
    UserInfo *onlineUserBuffer = readUserInfo(onlineUser.username); // 查找在线用户的信息
    displayText(lcd, FONT_PATH,
                155, 20, getColor(0, 255, 145, 85),
                0, 3, 20, 0,
                getColor(0, 255, 255, 255), onlineUserBuffer->username,
                172, 325);                                        // 显示在线用户名
    sprintf(NowBalanceBuffer, "%.2f", onlineUserBuffer->balance); // 更新当前登录用户实际转换 余额
    displayText(lcd, FONT_PATH,
                155, 20, getColor(0, 255, 145, 85),
                0, 3, 20, 0,
                getColor(0, 255, 255, 255), NowBalanceBuffer,
                172, 366); // 显示更新当前登录用户实际转换 余额
    displayText(lcd, FONT_PATH,
                155, 20, getColor(0, 255, 145, 85),
                0, 3, 20, 0,
                getColor(0, 255, 255, 255), onlineUserBuffer->created_at,
                172, 409); // 显示更新当前登录用户实际转换 余额
    /******默认显示刷新*******************/
    if (userLognum == 0) // 若日志为空, 则提示无日志
    {
        displayText(lcd, FONT_PATH,
                    392, 282, getColor(0, 255, 145, 85),
                    153, 107, 38, 0,
                    getColor(0, 255, 255, 255), "无日志",
                    365, 95);
    }
    else // 第一页已经加载
    {
        queryUserCashlogResult = queryUserCashlog(onlineUserBuffer->username, userLogCount, &log);

        /***更新日志缓冲区***/
        if (queryUserCashlogResult)
        {
            printf("预装载第一页日志失败\n");
            displayText(lcd, FONT_PATH,
                        392, 282, getColor(0, 255, 145, 85),
                        153, 107, 38, 0,
                        getColor(0, 255, 255, 255), "无日志",
                        365, 95);
        }
        else
        {
            sprintf(userLogCountStr, "%d/%d", userLogCount + 1, userLognum); // 将计数页面转换为字符串
            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 235, 214, 199),
                        0, 3, 18, 0,
                        getColor(0, 255, 255, 255), userLogCountStr,
                        508, 121); // 剩余页面

            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 246, 213, 187),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), log.chargingStationName,
                        508, 151); // 充电站显示
            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 238, 222, 185),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), log.model,
                        508, 181); // 设备型号
            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 237, 232, 206),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), log.remark,
                        508, 211); // 车辆名字
            /***********/
            formatTTaken(log.timeTaken, timeTakenBuffer, sizeof(timeTakenBuffer)); // 更新转换 充电时间
            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 229, 226, 194),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), timeTakenBuffer,
                        508, 241); // 充电用时

            sprintf(totalElectricityBuffer, "%.2f", log.totalElectricity); // 更新转换 充电电量
            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 219, 220, 194),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), totalElectricityBuffer,
                        508, 271);                       // 充电电量
            sprintf(balanceBuffer, "%.2f", log.balance); // 更新转换 消费额度
            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 198, 210, 188),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), balanceBuffer,
                        508, 301);                                                     // 消费额度
            formatConsumptionTime(log.consumption_at, timeBuffer, sizeof(timeBuffer)); // 更新转换 消费时间
            displayText(lcd, FONT_PATH,
                        180, 18, getColor(0, 211, 218, 211),
                        0, 0, 18, 0,
                        getColor(0, 255, 255, 255), timeBuffer,
                        508, 331); // 消费时间
        }
    }

    while (1)
    {

        // 点击返回
        if (isTouchButton(ts_x, ts_y, getback_Button))
        {
            state = state_SystemMain; // 返回系统主界面
            printf("用户点击返回, 返回系统主界面\n");
            free(onlineUserBuffer);
            break;
        }

        // 点击上一页
        if (isTouchButton(ts_x, ts_y, previous_Button))
        {
            if (userLogCount != 0 && userLogCount < userLognum)
            {
                userLogCount--;
                queryUserCashlog(onlineUserBuffer->username, userLogCount, &log);
                sprintf(userLogCountStr, "%d/%d", userLogCount + 1, userLognum); // 将计数页面转换为字符串
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 235, 214, 199),
                            0, 3, 18, 0,
                            getColor(0, 255, 255, 255), userLogCountStr,
                            508, 121); // 剩余页面

                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 246, 213, 187),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), log.chargingStationName,
                            508, 151); // 充电站显示
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 238, 222, 185),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), log.model,
                            508, 181); // 设备型号
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 237, 232, 206),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), log.remark,
                            508, 211); // 车辆名字
                /***********/
                formatTTaken(log.timeTaken, timeTakenBuffer, sizeof(timeTakenBuffer)); // 更新转换 充电时间
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 229, 226, 194),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), timeTakenBuffer,
                            508, 241); // 充电用时

                sprintf(totalElectricityBuffer, "%.2f", log.totalElectricity); // 更新转换 充电电量
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 219, 220, 194),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), totalElectricityBuffer,
                            508, 271);                       // 充电电量
                sprintf(balanceBuffer, "%.2f", log.balance); // 更新转换 消费额度
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 198, 210, 188),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), balanceBuffer,
                            508, 301);                                                     // 消费额度
                formatConsumptionTime(log.consumption_at, timeBuffer, sizeof(timeBuffer)); // 更新转换 消费时间
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 211, 218, 211),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), timeBuffer,
                            508, 331); // 消费时间
                CoordinateReset();
            }
        }
        /************END***************/

        // 点击下一页
        if (isTouchButton(ts_x, ts_y, next_Button))
        {
            if (userLognum > 1 && userLogCount <= userLognum - 2) // 存在2页以上日志 且没有超出范围
            {
                userLogCount++;
                queryUserCashlog(onlineUserBuffer->username, userLogCount, &log);
                sprintf(userLogCountStr, "%d/%d", userLogCount + 1, userLognum); // 将计数页面转换为字符串
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 235, 214, 199),
                            0, 3, 18, 0,
                            getColor(0, 255, 255, 255), userLogCountStr,
                            508, 121); // 剩余页面

                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 246, 213, 187),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), log.chargingStationName,
                            508, 151); // 充电站显示
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 238, 222, 185),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), log.model,
                            508, 181); // 设备型号
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 237, 232, 206),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), log.remark,
                            508, 211); // 车辆名字
                /***********/
                formatTTaken(log.timeTaken, timeTakenBuffer, sizeof(timeTakenBuffer)); // 更新转换 充电时间
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 229, 226, 194),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), timeTakenBuffer,
                            508, 241); // 充电用时

                sprintf(totalElectricityBuffer, "%.2f", log.totalElectricity); // 更新转换 充电电量
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 219, 220, 194),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), totalElectricityBuffer,
                            508, 271);                       // 充电电量
                sprintf(balanceBuffer, "%.2f", log.balance); // 更新转换 消费额度
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 198, 210, 188),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), balanceBuffer,
                            508, 301);                                                     // 消费额度
                formatConsumptionTime(log.consumption_at, timeBuffer, sizeof(timeBuffer)); // 更新转换 消费时间
                displayText(lcd, FONT_PATH,
                            180, 18, getColor(0, 211, 218, 211),
                            0, 0, 18, 0,
                            getColor(0, 255, 255, 255), timeBuffer,
                            508, 331); // 消费时间
                CoordinateReset();
            }
        }
        /************END***************/
    }
}
/***************************************充电价格展示******************************************/
/***************************************充电价格展示******************************************/
void handle_PriceList()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("PriceList充电价格选择\n");
    read_JPEG_file_LCD("../pic/PriceList.jpg", 0, 0, lcd->mp, 800);
    printf("充电价格界面刷新完成\n");
    // 装载按钮位置
    Button getback_Button = {19, 30, 49, 49}; // 返回按钮 x y 宽 高
    while (1)
    {
        // 点击返回
        if (isTouchButton(ts_x, ts_y, getback_Button))
        {
            state = state_SystemMain; // 返回系统主界面
            printf("用户点击返回, 返回系统主界面\n");
            break;
        }
    }

    /************END***************/
}
/***************************************充电模式设备选择******************************************/
/***************************************充电模式设备选择******************************************/
void handle_ChoosingChargingDev()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Choosing Charging Device充电模式设备选择\n");
    read_JPEG_file_LCD("../pic/ChoosingChargingDev.jpg", 0, 0, lcd->mp, 800);
    printf("充电模式设备选择界面刷新完成\n");
    // 装载按钮位置
    Button getback_Button = {0, 0, 30, 20};  // 返回按钮 x y 宽 高
    Button J_Button = {89, 219, 300, 180};   // J1772充电枪 慢充
    Button KJ_Button = {426, 219, 300, 180}; // KJH1
    bool continue_Button_flag = false;       // 控制继续按钮是否激活可用, 默认休眠
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    while (1)
    {
        // 点击返回
        if (isTouchButton(ts_x, ts_y, getback_Button))
        {
            state = state_SystemMain; // 返回系统主界面
            printf("用户点击返回, 返回系统主界面\n");
            break;
        }

        // 选择J1772充电枪
        if (isTouchButton(ts_x, ts_y, J_Button))
        {
            strncpy(selectChargerResult, "9240Aier", sizeof(selectChargerResult) - 1);
            // 将选择结果储存
            printf("用户选择了9240Aier充电枪\n");
            /************/
            /*****显示选择结果******/
            CoordinateReset();           // 清除坐标
            continue_Button_flag = true; // 激活继续按钮
        }
        else if (isTouchButton(ts_x, ts_y, KJ_Button))
        {
            strncpy(selectChargerResult, "1891JiAixi", sizeof(selectChargerResult) - 1);
            printf("用户选择了1891JiAixi充电枪\n");
            /************/
            /*****显示选择结果******/
            CoordinateReset();           // 清除坐标
            continue_Button_flag = true; // 激活继续按钮
        }
        if (continue_Button_flag == true) // 在激活状态下有效点击
        {
            // continue_Button_flag = false;
            state = state_PromptCharger; // 进入提示插好充电枪界面
            printf("用户有效点击继续,用户选择了%s,进入提示插好充电枪界面\n", selectChargerResult);
            break;
        }
    }

    /************END***************/
}
/***************************************提示:充电模式选择 请插好枪******************************************/
/***************************************提示:充电模式选择 请插好枪******************************************/
void handle_PromptCharger()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Prompt Charger提示:充电模式选择   请插好枪\n");
    read_JPEG_file_LCD("../pic/PromptCharger.jpg", 0, 0, lcd->mp, 800);
    printf("提示:充电模式选择 请插好枪 刷新完成\n");
    // 装载按钮位置
    Button getback_Button = {0, 0, 30, 20};     // 返回按钮 x y 宽 高
    Button start_Button = {250, 240, 300, 100}; // 开始充电按钮
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    while (1)
    {
        // 点击返回
        if (isTouchButton(ts_x, ts_y, getback_Button))
        {
            state = state_ChoosingChargingDev; // 返回充电枪型号选择界面
            printf("用户点击返回, 返回充电枪型号选择界面\n");
            break;
        }
        // 点击开始充电
        if (isTouchButton(ts_x, ts_y, start_Button))
        {
            state = state_InCharge; // 进入充电中界面
            printf("用户点击开始充电, 进入充电中界面\n");
            break;
        }
    }
    /************END***************/
}
/***************************************充电中界面******************************************/
/***************************************充电中界面******************************************/
void handle_InCharge()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling In Charge 充电中界面\n");
    read_JPEG_file_LCD("../pic/InCharge.jpg", 0, 0, lcd->mp, 800);
    printf("充电中界面 刷新完成\n");
    topupMoney = 0;

    /******************************局部变量****************************/
    /*********初始化相关信息*******/
    srand(time(NULL)); // 使用当前时间作为随机数生成器的种子 这确保了每次程序运行时生成的随机数序列不同
    /*****车辆信息随机生成****/
    // 车辆参数, 后面有硬件的话, 直接替换这一部分即可
    Car car;
    int carserialNumber = generateRandomInRange(0, 9);               // 车辆序列号(随机0~9)
    snprintf(car.name, sizeof(car.name), CarsName[carserialNumber]); // 生成 汽车名称 Car x
    car.batteryCapacity = randomFloat(80.0, 120.0);                  // 生成 随机电池容量 在 50 到 100 kWh 之间
    car.currentCharge = randomFloat(0.0, 30.0);                      // 生成 随机当前电量在 0 到电池容量之间
                                                                     /*****END****/
    printf("车已到站, 名称: %s\n\t电池容量: %.2fKWh\n\t当前电量: %.2fKWh\n", car.name, car.batteryCapacity, car.currentCharge);
    /*************************************************/

    chargingSession.car = car; // 记录正在充电车辆的参数
    // 遍历 chargers 数组，找到匹配的充电枪型号
    for (int i = 0; i < sizeof(chargers) / sizeof(chargers[0]); i++)
    {
        if (strcmp(selectChargerResult, chargers[i].model) == 0)
        {
            // 找到匹配的充电枪型号，复制参数到 chargingSession.charger
            chargingSession.charger = chargers[i];
            break;
        }
    }
    /***测试用***/
    // 打印选择的充电枪信息
    printf("选择的充电枪型号: %s\n", chargingSession.charger.model);
    printf("最大充电功率: %.2f kW\n", chargingSession.charger.maxPower);
    printf("充电电压: %.2f V\n", chargingSession.charger.voltage);
    printf("充电电流: %.2f A\n", chargingSession.charger.current);
    /****END***/
    // 计算充满需要多少度电
    float chargedEnergy_Temp = chargingSession.car.batteryCapacity - chargingSession.car.currentCharge; // 应该充入电量=车型号电量-当前电量
    // 大于3000是会员
    /************判断在线用户是不是会员*************/
    char NowBalanceBuffer[50]; // 存放在线用户余额字符串转换结果
    /****在线用户信息刷新**********/
    UserInfo *onlineUserBuffer = readUserInfo(onlineUser.username); // 查找在线用户的信息
    bool isVIP_flag = false;                                        // false不是会员,true是会员
    if (onlineUserBuffer->balance >= 3000)
    {
        printf("用户是会员!优惠一些,留住他!o(*￣︶￣*)o\n");
        isVIP_flag = true;
    }
    else
    {
        printf("用户是不是会员!原价计算~\n");
    }
    /************END*************/
    /***********判断当前是不是高峰期***********/

    bool isPeakTime_flag = true; // false不是高峰期,true是高峰期
    if (isPeakTime())
    {
        printf("现在是高峰期\n");
        isPeakTime_flag = true;
    }
    else
    {
        printf("现在是非高峰期\n");
        isPeakTime_flag = false;
    }
    /*********END***************/
    char estimatedEndTimeStr[50]; // 预计充电完成时间
    // 预计完成需要多少秒
    float Estimatedtime = calculateEstimatedEndTime(chargingSession.car.batteryCapacity,
                                                    chargingSession.chargedEnergy,
                                                    chargingSession.charger.maxPower,
                                                    estimatedEndTimeStr);
    printf("预计完成需要 %.2f 分钟\n", Estimatedtime);
    printf("预计完成需要时分秒: %s\n", estimatedEndTimeStr);

    // 充电开始的时间
    chargingSession.startTime = time(NULL);
    // 已经充电的时间(秒)
    int useTime_s;
    // 全部充满需要多少钱 注意用于算法是按照实际时间编写的 故而记得去头文件打开所需模式, 默认测试模式
    float costFull = 60 * calculateChargingCost(chargingSession.chargedEnergy, isPeakTime_flag, isVIP_flag); // 如果充满的全部的费用

    float cost_per_minute = costFull / Estimatedtime;                    // 每秒的费用(测试模式是每分钟)
    float cost_Temp = 0;                                                 // 已经产生的费用
    float inKilowattHour = 0;                                            // 已经充入的电量(度)
    float chargePercentage_Temp = 0;                                     // 已经充入电量的百分百比
    float chargedEnergy_per_minute = chargedEnergy_Temp / Estimatedtime; // 每秒充入的电量KWh (测试模式是每分钟)

    // 模拟充电过程中电压电流的小波动,但对充电功率没有影响  因为没有硬件
    float voltageFluctuation = 0; // 电压值动态变化
    float currentFluctuation = 0; // 电流值动态变化

    /************************************************/
    // 装载按钮位置
    Button stop_Button = {562, 355, 153, 42}; // 停止充电按钮 x y 宽 高
    bool stopTrue_flag = false;               // 确认停止 false默认
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    while (1)            // 充电过程实现 微积分太难, 就梯度判断
    {
        /*******退出条件*********/
        // 点击停止充电
        if (isTouchButton(ts_x, ts_y, stop_Button))
        {
            CoordinateReset();    // 坐标清空
            stopTrue_flag = true; // 激活二次停止确认
        }
        // 确认 停止充电
        if (stopTrue_flag == true)
        {
            state = state_ChargingEndInfo; // 进入停止充电确认界面
            printf("用户点击停止充电, 返回充电枪型号选择界面\n");

            // 储存有用信息

            chargingSession.chargedEnergy = inKilowattHour;           // 更新已经充入的电量KWh
            chargingSession.chargingCost = cost_Temp;                 // 更新本次充电花费费用
            chargingSession.chargePercentage = chargePercentage_Temp; // 更新汽车现在的电量状态百分百比
            chargingSession.useTime = useTime_s;                      // 更新本次充电的使用时间
            // 打印 chargingSession 的内容  调试检查是否储存所有的信息
            printf("即将退出,开始检查所有信息:\n");
            printChargingSession(&chargingSession);
            return;
        }

        if (inKilowattHour < chargedEnergy_Temp)
        {
            /***剩余电量动态变化计算***/
            cost_Temp += cost_per_minute;               // 累加已经产生的费用
            inKilowattHour += chargedEnergy_per_minute; // 累加已经充入的电量
            if (inKilowattHour > chargedEnergy_Temp)    // 如果超充
            {
                inKilowattHour = chargedEnergy_Temp;
            }

            // 已经充入电量的百分百比            (已经充入的电量+原来有的电)/总电量 *100
            chargePercentage_Temp = ((inKilowattHour + chargingSession.car.currentCharge) / chargingSession.car.batteryCapacity) * 100;
            // 更新电压电流
            voltageFluctuation = chargingSession.charger.voltage + randomFloat(-2.0, 2.0);   // 电压波动量
            currentFluctuation = chargingSession.charger.current + randomFloat(-10.0, 10.0); // 电流波动量
                                                                                             // 更新已经使用时间 (秒)(测试模式是每分钟)
            useTime_s = (time(NULL) - chargingSession.startTime) * 60;                       // 加速60倍

            printf("电压: %.2f V, 电流: %.2f A\n", voltageFluctuation, currentFluctuation);
            if (chargePercentage_Temp <= 100)
            {
                char cost_TempStr[100]; // 已经产生的费用 转换字符串
                sprintf(cost_TempStr, "%.2f", cost_Temp);

                char inKilowattHourStr[100]; // 充入的电量KWh 转换字符串
                sprintf(inKilowattHourStr, "%.2f", inKilowattHour);

                char voltageFluctuation_TempStr[100]; // 电压波动量 转换字符串
                sprintf(voltageFluctuation_TempStr, "%.2f", voltageFluctuation);

                char currentFluctuation_TempStr[100]; // 电流波动量 转换字符串
                sprintf(currentFluctuation_TempStr, "%.2f", currentFluctuation);

                char chargePercentage_TempStr[100]; // 充电百分比 转换字符串
                sprintf(chargePercentage_TempStr, "%.2f %%", chargePercentage_Temp);

                char useTimeBuffer_TempStr[100]; // 已经使用的时间 转换字符串
                formatTTaken(useTime_s, useTimeBuffer_TempStr, sizeof(useTimeBuffer_TempStr));
                displayText(lcd, FONT_PATH,
                            90, 30, getColor(0, 255, 145, 85),
                            0, 3, 28, 0,
                            getColor(0, 255, 255, 255), chargePercentage_TempStr,
                            600, 260); // 实时百分比
                displayText(lcd, FONT_PATH,
                            70, 32, getColor(0, 255, 145, 85),
                            20, 13, 20, 0,
                            getColor(0, 255, 255, 255), voltageFluctuation_TempStr,
                            339, 315); // 实时电压
                displayText(lcd, FONT_PATH,
                            70, 32, getColor(0, 255, 145, 85),
                            20, 13, 20, 0,
                            getColor(0, 255, 255, 255), currentFluctuation_TempStr,
                            136, 315); // 实时电流

                displayText(lcd, FONT_PATH,
                            70, 32, getColor(0, 255, 145, 85),
                            20, 13, 20, 0,
                            getColor(0, 255, 255, 255), inKilowattHourStr,
                            136, 182); // 充入电量

                displayText(lcd, FONT_PATH,
                            70, 32, getColor(0, 255, 145, 85),
                            20, 13, 20, 0,
                            getColor(0, 255, 255, 255), cost_TempStr,
                            339, 182); // 充入费用

                displayText(lcd, FONT_PATH,
                            90, 32, getColor(0, 255, 145, 85),
                            0, 12, 20, 0,
                            getColor(0, 255, 255, 255), useTimeBuffer_TempStr,
                            600, 145); // 充电计时
                // displayText(lcd, FONT_PATH,
                //             201, 38, getColor(0, 255, 145, 85),
                //             0, 3, 20, 0,
                //             getColor(0, 255, 255, 255), estimatedEndTimeStr,
                //             591, 318); // 预计需要多少时间
                printf("已经产生的费用: %.2f 元, 已经充入的电量: %.2f KWh, 汽车现在电量的百分百比: %.2f %%\n", cost_Temp, inKilowattHour, chargePercentage_Temp);
            }
            if (chargePercentage_Temp > 100)
            {
                chargePercentage_Temp = 100;

                char chargePercentage_TempStr[100]; // 充电百分比 转换字符串
                sprintf(chargePercentage_TempStr, "%.2f %%", chargePercentage_Temp);
                displayText(lcd, FONT_PATH,
                            90, 30, getColor(0, 255, 145, 85),
                            0, 3, 28, 0,
                            getColor(0, 255, 255, 255), chargePercentage_TempStr,
                            600, 260); // 实时百分比
            }

            printf("\n");
        }
        else
        {
            chargePercentage_Temp = 100;
            inKilowattHour = chargedEnergy_Temp;
            currentFluctuation = 0;
        } // 自动停止

        sleep(1); // 人的神经反射最快0.1s, 故而每1秒更新一次数据 表示已经充了1分钟 后面要*60还原  用户长安1秒停止按钮结束充电
    }
    /************END***************/
    /*********信息保存**********/
}
/***************************************充电结束信息界面******************************************/
/***************************************充电结束信息界面******************************************/
void handle_ChargingEndInfo()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Charging End Info充电结束信息\n");
    read_JPEG_file_LCD("../pic/ChargingEndInfo.jpg", 0, 0, lcd->mp, 800);
    printf("充电结束信息界面刷新完成\n");
    // 装载按钮位置
    Button OK_Button = {284, 393, 229, 53}; // OK按钮 x y 宽 高
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    printf("即将展示,开始检查所有信息:\n");
    printChargingSession(&chargingSession);
    /**********************显示本次的消费信息*************************/
    {
        // char chargePercentage_TempStr[100]; // 充电百分比 转换字符串
        // sprintf(chargePercentage_TempStr, "%.2f%%", chargingSession.chargePercentage);
        // displayText(lcd, FONT_PATH,
        //             63, 40, getColor(0, 19, 32, 31),
        //             0, 3, 25, 0,
        //             getColor(0, 103, 218, 41), chargePercentage_TempStr,
        //             59, 148); // 百分比
        /*r2*****************************/
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 226, 214, 212),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), chargingSession.car.name,
                    531, 198); // 车名称
        /*r1*****************************/
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 209, 206, 232),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), chargingSession.charger.model,
                    531, 146); // 选择的充电枪
        /*l1*************************/
        char voltage_TempStr[100]; // 充电枪的充电电压 转换字符串
        sprintf(voltage_TempStr, "%.2f V", chargingSession.charger.voltage);
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 206, 222, 239),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), voltage_TempStr,
                    212, 146); // 充电枪的充电电压
        /*l2*************************/
        char current_TempStr[100]; // 充电枪的充电电流 转换字符串
        sprintf(current_TempStr, "%.2f A", chargingSession.charger.current);
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 208, 239, 210),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), current_TempStr,
                    212, 198); // 充电枪的充电电流
        /*r3*************************/
        char inKilowattHourStr[100]; // 充入的电量KWh 转换字符串
        sprintf(inKilowattHourStr, "%.2fKWh", chargingSession.chargedEnergy);
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 206, 222, 239),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), inKilowattHourStr,
                    531, 251); // 充入电量
        /*l3*************************/
        char startTime[100]; // 开始充电时间
        struct tm *timeinfo;
        timeinfo = localtime(&chargingSession.startTime);
        // 格式化成年月日的形式
        snprintf(startTime, sizeof(startTime), "%04d-%02d-%02d",
                 timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
        // 然后在显示文本的地方使用 startTime 变量
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 232, 233, 194),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), startTime,
                    212, 251); // 显示当前的年月日
        /*l4*************************/
        char useTimeBuffer_TempStr[100]; // 已经使用的时间 转换字符串
        formatTTaken(chargingSession.useTime, useTimeBuffer_TempStr, sizeof(useTimeBuffer_TempStr));
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 239, 213, 197),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), useTimeBuffer_TempStr,
                    212, 305); // 充电计时
        /*r4*************************/
        char cost_TempStr[100]; // 已经产生的费用 转换字符串
        sprintf(cost_TempStr, "%.2f元", chargingSession.chargingCost);
        displayText(lcd, FONT_PATH,
                    149, 35, getColor(0, 239, 213, 197),
                    0, 3, 33, 0,
                    getColor(0, 103, 218, 41), cost_TempStr,
                    531, 305); // 产生的费用
    }
    /****************************END*************************/
    while (1)
    {
        // 点击OK
        if (isTouchButton(ts_x, ts_y, OK_Button))
        {
            state = state_PatternPayment; // 进入付费方式选择窗口
            printf("用户点击OK, 进入付费方式选择窗口\n");
            CoordinateReset();
            Payment_flag = 0;
            break;
        }
    }
    /************END***************/
}
/***************************************付费方式选择窗口******************************************/
/***************************************付费方式选择窗口******************************************/
void handle_PatternPayment()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Pattern Payment付费方式选择窗口\n");
    read_JPEG_file_LCD("../pic/PatternPayment.jpg", 0, 0, lcd->mp, 800);
    printf("付费方式选择窗口刷新完成\n");
    // 装载按钮位置
    Button getback_Button = {0, 0, 30, 20};          // 返回按钮 x y 宽 高
    Button balancePay_Button = {131, 243, 218, 108}; // 余额支付 x y 宽 高
    Button scanPay_Button = {464, 243, 218, 108};    // 扫码支付 x y 宽 高
    float feeDeductionResult = 0;                    // 支付扣费,用于余额支付(钱够的情况下),这是扣费后用户的余额||充值后还有多少钱
    // 查找当前用户信息
    UserInfo *onlineUserBuffer = readUserInfo(onlineUser.username); // 查找在线用户的信息
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    while (1)
    {
        if (Payment_flag == 0) // 从结算页面来的, 执行对应选项
        {
            // 点击返回 返回结算页面
            if (isTouchButton(ts_x, ts_y, getback_Button))
            {
                printf("用户点击返回, 返回结算页面\n");
                state = state_ChargingEndInfo; // 跳转到支付成功界面
                free(onlineUserBuffer);        // 临时读取的区域已经用完了, 没有作用了, 释放
                return;
            }
            // 点击余额支付
            if (isTouchButton(ts_x, ts_y, balancePay_Button))
            {
                /*计算用户余额情况与支付费用对比*/
                // 如果费用足够,扣费后重新写入
                if (onlineUserBuffer->balance >= chargingSession.chargingCost)
                {

                    feeDeductionResult = onlineUserBuffer->balance - chargingSession.chargingCost; // 还剩多少钱
                    printf("用户余额充足, 有%.2f元, 应该支付%.2f元,结余%.2f元\n", onlineUserBuffer->balance, chargingSession.chargingCost, feeDeductionResult);
                    // 更新在线用户(堆内存内缓冲区的用户镜像)的余额
                    onlineUserBuffer->balance = feeDeductionResult; // 含有创建日期,用于更新信息
                    onlineUser.balance = feeDeductionResult;        // 更新在线用户的余额 用feeDeductionResult是因为后面要测试用
                    // 更新在线用户信息文件, 在线用户的新信息  注意在线用户默认是没有创建时间的, 要用堆内存用户信息!
                    updateUserInfo(*onlineUserBuffer);
                    printf("更新用户%s信息数据成功\n", onlineUser.username);
                    UserInfo *onlineUserBufferUpdate = readUserInfo(onlineUser.username); // 再次读取用户数据
                    printf("此时用户%s的文件读取后的余额为%.2f元\n", onlineUserBufferUpdate->username, onlineUserBufferUpdate->balance);
                    printf("结算成功, 谢谢使用, 跳转支付结果页面\n");
                    state = state_PaymentResult; // 返回充值界面
                    free(onlineUserBufferUpdate);
                    free(onlineUserBuffer); // 临时读取的区域已经用完了, 没有作用了, 释放
                    return;
                }
                else // 余额不足情况,跳转到充值询问界面
                {
                    printf("余额不足,跳转到充值询问界面\n");
                    state = state_NotSufficientFunds; // 跳转到充值询问界面
                    free(onlineUserBuffer);           // 临时读取的区域已经用完了, 没有作用了, 释放
                    return;
                }
            }
            // 点击扫码支付
            if (isTouchButton(ts_x, ts_y, scanPay_Button))
            {
                printf("点击扫码支付, 跳转扫码支付界面\n");
                state = state_ScanPay;  // 跳转到支付成功界面
                free(onlineUserBuffer); // 临时读取的区域已经用完了, 没有作用了, 释放
                return;
            }
        }
        else if (Payment_flag == 1) // 从主界面来充值的
        {
            // 点击返回 返回充值界面
            if (isTouchButton(ts_x, ts_y, getback_Button))
            {
                printf("用户点击返回, 返回充值界面\n");
                state = state_RechargeInterface; // 返回充值界面
                free(onlineUserBuffer);          // 临时读取的区域已经用完了, 没有作用了, 释放
                return;
            }
            // 点击余额支付
            if (isTouchButton(ts_x, ts_y, balancePay_Button))
            {
                printf("用户从充值界面从余额, 不能余额冲余额, 无效点击\n");
            }
            // 点击扫码支付
            if (isTouchButton(ts_x, ts_y, scanPay_Button))
            {
                printf("充值过程中点击扫码支付, 跳转扫码支付界面\n");
                state = state_ScanPay;  // 跳转到扫码
                free(onlineUserBuffer); // 临时读取的区域已经用完了, 没有作用了, 释放
                return;
            }
        }
    }
    /************END***************/
}
/***************************************扫码支付******************************************/
/***************************************扫码支付******************************************/
void handle_ScanPay()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Scan Pay扫码支付\n");
    read_JPEG_file_LCD("../pic/ScanPay1.jpg", 0, 0, lcd->mp, 800);
    printf("充值界面刷新完成\n");
    // 生成随机数
    // int randScannumber = generateRandomInRange(0, 3);
    // char gif_path[128] = {0};                                  // 构造图片路径的缓存区
    // sprintf(gif_path, "../pic/scanpay%d.jpg", randScannumber); // 构造jpg图片的路径
    // read_JPEG_file_LCD(gif_path, 268, 108, lcd->mp, 800);      // 在LCD上显示二维码

    // 装载按钮位置
    Button getback_Button = {0, 0, 30, 20}; // 返回按钮 x y 宽 高/
    Button TruePay_Button = {587, 395, 175, 60};
    float feeDeductionResult = 0;                                   // 支付扣费,用于余额支付(钱够的情况下),这是扣费后用户的余额||充值后还有多少钱                                          // 自觉支付成功 x y 宽 高/
    UserInfo *onlineUserBuffer = readUserInfo(onlineUser.username); // 查找在线用户的信息
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    while (1)
    {
        // 点击返回 返回支付方式选择界面
        if (isTouchButton(ts_x, ts_y, getback_Button))
        {
            state = state_PatternPayment; // 进入付费方式选择窗口
            printf("用户点击返回, 返回付费方式选择窗口\n");
            CoordinateReset();
            break;
        }
        // 点击自觉支付成功 进入结果界面
        if (isTouchButton(ts_x, ts_y, TruePay_Button))
        {
            if (Payment_flag == 0 && topupMoney != 0)
            {
                onlineUserBuffer->balance = onlineUserBuffer->balance + topupMoney;
                if (onlineUserBuffer->balance >= chargingSession.chargingCost)
                {

                    feeDeductionResult = onlineUserBuffer->balance - chargingSession.chargingCost; // 还剩多少钱
                    printf("用户余额充足, 有%.2f元, 应该支付%.2f元,结余%.2f元\n", onlineUserBuffer->balance, chargingSession.chargingCost, feeDeductionResult);
                    // 更新在线用户(堆内存内缓冲区的用户镜像)的余额
                    onlineUserBuffer->balance = feeDeductionResult; // 含有创建日期,用于更新信息
                    onlineUser.balance = feeDeductionResult;        // 更新在线用户的余额 用feeDeductionResult是因为后面要测试用
                    // 更新在线用户信息文件, 在线用户的新信息  注意在线用户默认是没有创建时间的, 要用堆内存用户信息!
                    updateUserInfo(*onlineUserBuffer);
                    printf("更新用户%s信息数据成功\n", onlineUser.username);
                    UserInfo *onlineUserBufferUpdate = readUserInfo(onlineUser.username); // 再次读取用户数据
                    printf("此时用户%s的文件读取后的余额为%.2f元\n", onlineUserBufferUpdate->username, onlineUserBufferUpdate->balance);
                    printf("结算成功, 谢谢使用, 跳转支付结果页面\n");
                    state = state_PaymentResult; // 返回充值界面
                    free(onlineUserBufferUpdate);
                    free(onlineUserBuffer); // 临时读取的区域已经用完了, 没有作用了, 释放
                    return;
                }
                else // 余额不足情况,跳转到充值询问界面
                {
                    printf("余额不足,跳转到充值询问界面\n");
                    state = state_NotSufficientFunds; // 跳转到充值询问界面
                    free(onlineUserBuffer);           // 临时读取的区域已经用完了, 没有作用了, 释放
                    return;
                }
            }
            else if (Payment_flag == 1)
            {
                feeDeductionResult = onlineUserBuffer->balance + topupMoney;
                // 更新在线用户(堆内存内缓冲区的用户镜像)的余额
                onlineUserBuffer->balance = feeDeductionResult; // 含有创建日期,用于更新信息
                onlineUser.balance = feeDeductionResult;        // 更新在线用户的余额 用feeDeductionResult是因为后面要测试用
                // 更新在线用户信息文件, 在线用户的新信息  注意在线用户默认是没有创建时间的, 要用堆内存用户信息!
                updateUserInfo(*onlineUserBuffer);
                printf("更新用户%s信息数据成功\n", onlineUser.username);
                UserInfo *onlineUserBufferUpdate = readUserInfo(onlineUser.username); // 再次读取用户数据
                printf("此时用户%s的文件读取后的余额为%.2f元\n", onlineUserBufferUpdate->username, onlineUserBufferUpdate->balance);
                printf("结算成功, 谢谢使用, 跳转支付结果页面\n");
                state = state_PaymentResult; // 返回充值界面
                free(onlineUserBufferUpdate);
                free(onlineUserBuffer); // 临时读取的区域已经用完了, 没有作用了, 释放
            }
            state = state_PaymentResult; // 进入支付结果选择窗口
            printf("点击自觉支付成功 进入结果界面\n");
            CoordinateReset();
            break;
        }
    }
    /************END***************/
}

/***************************************提示:余额不足******************************************/
/***************************************提示:余额不足******************************************/
void handle_NotSufficientFunds()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Not Sufficient Funds 提示:余额不足\n");
    read_JPEG_file_LCD("../pic/NotSufficientFunds.jpg", 0, 0, lcd->mp, 800);
    printf("提示:余额不足界面刷新完成\n");

    // 装载按钮位置
    Button getback_Button = {0, 0, 30, 20};      // 返回按钮 x y 宽 高/
    Button TruePay_Button = {321, 337, 206, 68}; // 愿意去支付 x y 宽 高/
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    while (1)
    {
        // 点击返回 或不想支付 返回支付方式选择界面
        if (isTouchButton(ts_x, ts_y, getback_Button))
        {
            state = state_PatternPayment; // 进入付费方式选择窗口
            printf("用户点击返回或不想支付, 返回付费方式选择窗口\n");
            CoordinateReset();
            break;
        }
        // 点击是 进入充值界面
        if (isTouchButton(ts_x, ts_y, TruePay_Button))
        {
            state = state_RechargeInterface; // 进入充值界面
            printf("点击自觉支付成功 进入充值界面\n");
            CoordinateReset();
            break;
        }
    }

    /************END***************/
}

/***************************************充值界面   未完成 键盘未接入******************************************/
/***************************************充值界面******************************************/
void handle_RechargeInterface()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Recharge Interface充值界面\n");
    read_JPEG_file_LCD("../pic/RechargeInterface.jpg", 0, 0, lcd->mp, 800);
    printf("充值界面刷新完成\n");
    topupMoney = 0;
    // 装载按钮位置
    Button getback_Button = {359, 368, 170, 57}; // 返回按钮 x y 宽 高/
    Button V50_Button = {57, 252, 115, 60};      // 50元按钮 x y 宽 高
    Button V100_Button = {205, 252, 115, 60};    // 100元按钮 x y 宽 高
    Button VOther_Button = {59, 343, 265, 60};   // 其它金额按钮 x y 宽 高 /
    Button OK_Button = {591, 368, 171, 60};      // 确认按钮 x y 宽 高 /

    /**************查询余额***********/
    char NowBalanceBuffer[50];                                      // 存放在线用户余额字符串转换结果
    UserInfo *onlineUserBuffer = readUserInfo(onlineUser.username); // 查找在线用户的信息
    sprintf(NowBalanceBuffer, "%.2f", onlineUserBuffer->balance);   // 更新当前登录用户实际转换 余额

    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称
    displayText(lcd, FONT_PATH,
                151, 31, getColor(0, 255, 145, 85),
                5, 8, 25, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                171, 70); // 显示当前登录用户名称
    displayText(lcd, FONT_PATH,
                151, 31, getColor(0, 255, 145, 85),
                5, 8, 25, 0,
                getColor(0, 103, 218, 41), NowBalanceBuffer,
                171, 112); // 查询充值
    free(onlineUserBuffer);
    /************END***************/
    bool KBDormancy_flag = false; // 键盘状态控制, false表示键盘当前不可用, true表示键盘可用. 默认不可用
    /************函数变量***************/
    /**初始化局部变量***/
    backspaceflag = false;    // 退格标志默认没有发生退格
    Stack s;                  // 创建键盘栈缓冲区
    initStack(&s);            // 初始化键盘栈
    char input;               // 存放一次读取的一个字符
    int x, y;                 // 存放当前坐标
    char Account_result[MAX]; // 存放金额的临时输入结果
    char *endptr;             // 存放字符转浮点的报错结果
    /************END***************/
    /************动作***************/
    printf("进入循环, 等待响应 Payment_flag是%d\n", Payment_flag);
    char Money_TempStr[100]; // 充值显示 转换字符串
    printf("进入循环, 等待响应\n");

    /************END***************/
    /************函数变量***************/
    /**初始化局部变量***/

    /************END***************/
    /************动作***************/
    printf("进入循环, 等待响应 Payment_flag是%d\n", Payment_flag);
    while (1)
    {
        /************页面直接跳转条件***************/
        // 点击返回 返回主菜单
        if (isTouchButton(ts_x, ts_y, getback_Button))
        {
            if (Payment_flag == 1)
            {
                printf("用户从菜单点击过来充值, 现在用户点击返回, 返回主菜单\n");
                state = state_SystemMain; // 返回充值界面
                return;
            }
            else if (Payment_flag == 0)
            {
                printf("用户充完电余额不足, 选择充值, 现在用户点击返回, 返回充电结束信息页面\n");
                state = state_ChargingEndInfo; // 返回充电结束信息页面
                return;
            }
        }
        // 点击确定, 进入结果页面
        if (isTouchButton(ts_x, ts_y, OK_Button))
        {

            printf("用户点击确定\n");
            if (Payment_flag == 1 && topupMoney != 0)
            {
                printf("用户从菜单点击过来充值, 现在用户点击返回, 返回主菜单\n");
                state = state_PatternPayment; // 进入结果页面
                return;
            }
            else if (Payment_flag == 0 && topupMoney != 0)
            {
                /************
          `       *
                 *若钱不够扣费, 则不能执行扣费成功, 让继续充值
                 * 这里将用户的充入的值减去应该扣的钱,然后更新到文件
                 *
                 *时间有限, 不做处理, 直接跳转到支付结果!!!!!!!!!!!!
                 */
                printf("用户充完电余额不足, 选择充值, 现在用户点击返回, 返回充电结束信息页面\n");
                state = state_PatternPayment; // 进入结果页面
                return;
            }
        }
        // 点击50, 时间有限,直接进入充值模块
        if (isTouchButton(ts_x, ts_y, V50_Button))
        {
            topupMoney = 50;
            //    break;
        }
        // 点击100, 时间有限,直接进入充值模块
        if (isTouchButton(ts_x, ts_y, V100_Button))
        {
            topupMoney = 100;
            //    break;
        }
        // 点击其它金额, 时间有限, 从控制台获取
        if (isTouchButton(ts_x, ts_y, VOther_Button))
        {
            printf("用户有效点击其它金额, 键盘已激活, 可输入!:\n");
            displayText(lcd, FONT_PATH,
                        132, 54, getColor(0, 0, 0, 255),
                        23, 17, 20, 0,
                        getColor(0, 255, 255, 255), "请输入金额",
                        190, 343);
            KBDormancy_flag = true; // 激活键盘

            while (1)
            {
                int x_bk = ts_x;
                int y_bk = ts_y;
                if (KBDormancy_flag == true)
                {
                    input = getNumberFromCoordinates2(ts_x, ts_y, &s); // 获取临时输入一个字符
                    if (s.top != 4 && input != '\0')
                    {
                        printf("金额框输入了 %c\n", input);
                        push(&s, input);                     // 输入有效,压栈  这里放入显示
                        getStackContent(&s, Account_result); // 更新账号缓冲区
                        displayText(lcd, FONT_PATH,
                                    132, 54, getColor(0, 255, 255, 255),
                                    0, 3, 48, 0,
                                    getColor(0, 156, 220, 254), Account_result,
                                    190, 343); // 刷新显示金额
                    }
                }
                if (backspaceflag == true)
                {

                    getStackContent(&s, Account_result); // 更新账号缓冲区
                    displayText(lcd, FONT_PATH,
                                132, 54, getColor(0, 255, 255, 255),
                                0, 3, 48, 0,
                                getColor(0, 156, 220, 254), Account_result,
                                190, 343); // 刷新显示金额
                    printf("金额输入过程中退格处理完成\n");
                    backspaceflag = false;
                }
                /*********状态切换条件**********/
                if (isTouchButton(x_bk, y_bk, getback_Button)) // 点击返回
                {
                    CoordinateReset();
                    printf("用户在输入金额过程中点击返回按钮, 返回身份选择界面\n");

                    if (Payment_flag == 1)
                    {
                        printf("用户从菜单点击过来充值, 现在用户点击返回, 返回主菜单\n");
                        state = state_SystemMain; // 返回充值界面
                        return;
                    }
                    else if (Payment_flag == 0)
                    {
                        printf("用户充完电余额不足, 选择充值, 现在用户点击返回, 返回充电结束信息页面\n");
                        state = state_ChargingEndInfo; // 返回充电结束信息页面
                        return;
                    }
                }
                if (isTouchButton(x_bk, y_bk, OK_Button)) // 点击确定
                {
                    CoordinateReset();
                    printf("用户在输入金额过程中点击确定按钮, 跳转进入确定界面\n");
                    topupMoney = strtod(Account_result, &endptr);
                    // 检查转换是否成功
                    if (*endptr != '\0')
                    {
                        printf("转换错误，不可转换部分: %s\n", endptr);
                        return;
                    }
                    if (Payment_flag == 1 && topupMoney != 0)
                    {
                        printf("用户从菜单点击过来充值, 现在用户点击返回, 返回主菜单\n");
                        state = state_PatternPayment; // 进入付费窗口选择页面
                        return;
                    }
                    else if (Payment_flag == 0 && topupMoney != 0)
                    {
                        /************
                  `       *
                         *若钱不够扣费, 则不能执行扣费成功, 让继续充值
                         * 这里将用户的充入的值减去应该扣的钱,然后更新到文件
                         *
                         *时间有限, 不做处理, 直接跳转到支付结果!!!!!!!!!!!!
                         */
                        printf("用户充完电余额不足, 选择充值, 现在用户点击返回, 返回充电结束信息页面\n");
                        state = state_PatternPayment; // 进入结果页面
                        topupMoney = strtod(Account_result, &endptr);
                        // 检查转换是否成功
                        if (*endptr != '\0')
                        {
                            printf("转换错误，不可转换部分: %s\n", endptr);
                            return;
                        }

                        return;
                    }
                }

                /*********END**********/
                CoordinateReset();
                NOPREEMPTION;
            }
            topupMoney = strtod(Account_result, &endptr);
            // 检查转换是否成功
            if (*endptr != '\0')
            {
                printf("转换错误，不可转换部分: %s\n", endptr);
                return;
            }
            printf("已退出金额循环获取\n");
        }
        /************END***************/
        /***********
         *
         * 将登录键盘接入即可
         *
         *
         */
        /************END***************/
        sprintf(Money_TempStr, "%.2f元", topupMoney);
        displayText(lcd, FONT_PATH,
                    90, 28, getColor(0, 255, 145, 85),
                    5, 3, 25, 0,
                    getColor(0, 103, 218, 41), Money_TempStr,
                    243, 196); // 显示充值点击的值
        CoordinateReset();
        usleep(400000); // 0.4秒检测一次
    }
    /****更新用户余额****/

    /************写入用户充值日志*************/
    printf("退出界面\n");
}

/***************************************支付结果******************************************/
/***************************************支付结果******************************************/
void handle_PaymentResult()
{
    /************动作***************/
    // 进来刷新一次界面
    printf("Handling Payment Result支付结果\n");
    read_JPEG_file_LCD("../pic/PaymentResult.jpg", 0, 0, lcd->mp, 800);
    printf("支付结果界面刷新完成\n");
    // 装载按钮位置

    Button OK_Button = {328, 371, 335, 64};                               // 确认按钮 x y 宽 高 /
    UserCashlog log;                                                      // 用户日志的缓存区
    UserInfo *onlineUserBufferUpdate = readUserInfo(onlineUser.username); // 读取用户数据

    strcpy(log.username, onlineUser.username);      // 更新当前登录账户的信息
    log.consumption_at = chargingSession.startTime; // 更新消费时间
    log.sum = chargingSession.chargingCost;         // 产生费用
    log.balance = onlineUserBufferUpdate->balance;
    strcpy(log.chargingStationName, "能量驿站01");
    strcpy(log.model, chargingSession.charger.model);
    log.timeTaken = chargingSession.useTime;
    log.totalElectricity = chargingSession.chargedEnergy;
    strcpy(log.remark, chargingSession.car.name);

    addUserCashlog(&log);
    displayText(lcd, FONT_PATH,
                90, 20, getColor(255, 255, 145, 85),
                0, 0, 18, 0,
                getColor(0, 255, 255, 255), onlineUser.username,
                700, 4); // 显示当前登录用户名称

    while (1)
    {
        /************页面直接跳转条件***************/
        // 点击返回 返回主菜单
        if (isTouchButton(ts_x, ts_y, OK_Button))
        {
            printf("现在用户点击支付成功, 返回主菜单\n");
            state = state_SystemMain; // 返回充值界面
            CoordinateReset();
            free(onlineUserBufferUpdate);
            return;
        }
    }

    /************END***************/
}
/***************************************END******************************************/
/***************************************键盘处理******************************************/
// 初始化键盘栈
void initStack(Stack *s)
{
    s->top = -1;
}
// 检查键盘栈是否为空 1表示空 0不空
int isEmpty(Stack *s)
{
    return s->top == -1;
}
// 检查键盘栈是否满
int isFull(Stack *s)
{
    return s->top == MAX - 1;
}
// 键盘入栈操作
void push(Stack *s, char item)
{
    if (isFull(s))
    {
        printf("键盘栈已满!\n");
    }
    else
    {
        s->items[++(s->top)] = item;
    }
}
// 键盘出栈操作
char pop(Stack *s)
{
    if (isEmpty(s))
    {
        printf("键盘栈空!\n");
        return '\0';
    }
    else
    {
        return s->items[(s->top)--];
    }
}
// 清栈操作函数
void clearStack(Stack *s)
{
    s->top = -1;
}
// 获取键盘栈的内容作为字符串 s栈 str储存结果的字符数组
void getStackContent(Stack *s, char *str)
{
    int i;
    for (i = 0; i <= s->top; i++)
    {
        str[i] = s->items[i];
    }
    str[i] = '\0';
}
// 将输入坐标翻译为字符数字 并执行退格 且坐标复位
char getNumberFromCoordinates(int x, int y, Stack *s)
{
    ts_x = -1;
    ts_y = -1; // 坐标复位
    if ((x >= 333 && y >= 154) && (x <= 333 + 90 && y <= 154 + 74))
    {
        // 退格键
        printf("退格\n");
        pop(s);
        backspaceflag = true;
        concealPassword_flag = true;
    }
    if ((x >= 333 && y >= 246) && (x <= 333 + 90 && y <= 246 + 74)) // 点击清屏按钮
    {
        printf("清屏\n");
        clearStack(s);
        backspaceflag = true;
        concealPassword_flag = true;
    }
    if ((x >= 68 && y >= 63) && (x <= 68 + 68 && y <= 63 + 75))
        return '1';
    if ((x >= 156 && y >= 63) && (x <= 156 + 68 && y <= 63 + 75))
        return '2';
    if ((x >= 243 && y >= 63) && (x <= 243 + 68 && y <= 63 + 75))
        return '3';
    if ((x >= 68 && y >= 154) && (x <= 68 + 68 && y <= 154 + 75))
        return '4';
    if ((x >= 156 && y >= 154) && (x <= 156 + 68 && y <= 154 + 75))
        return '5';
    if ((x >= 243 && y >= 154) && (x <= 243 + 68 && y <= 154 + 75))
        return '6';
    if ((x >= 68 && y >= 246) && (x <= 68 + 68 && y <= 246 + 75))
        return '7';
    if ((x >= 156 && y >= 246) && (x <= 156 + 68 && y <= 246 + 75))
        return '8';
    if ((x >= 243 && y >= 246) && (x <= 243 + 68 && y <= 246 + 75))
        return '9';
    if ((x >= 68 && y >= 338) && (x <= 68 + 68 && y <= 338 + 75))
        return '0';
    return '\0'; // 无效坐标
}

// 将输入坐标翻译为字符数字 并执行退格 且坐标复位
char getNumberFromCoordinates2(int x, int y, Stack *s)
{
    ts_x = -1;
    ts_y = -1; // 坐标复位
    if ((x >= 635 && y >= 273) && (x <= 635 + 112 && y <= 273 + 74))
    {
        // 退格键
        printf("退格\n");
        pop(s);
        backspaceflag = true;
        concealPassword_flag = true;
    }
    if ((x >= 375 && y >= 37) && (x <= 375 + 112 && y <= 37 + 55))
        return '1';
    if ((x >= 504 && y >= 37) && (x <= 504 + 112 && y <= 37 + 55))
        return '2';
    if ((x >= 635 && y >= 37) && (x <= 635 + 112 && y <= 37 + 55))
        return '3';
    if ((x >= 375 && y >= 116) && (x <= 375 + 112 && y <= 116 + 55))
        return '4';
    if ((x >= 504 && y >= 154) && (x <= 504 + 112 && y <= 116 + 55))
        return '5';
    if ((x >= 635 && y >= 154) && (x <= 635 + 112 && y <= 116 + 55))
        return '6';
    if ((x >= 375 && y >= 193) && (x <= 375 + 112 && y <= 193 + 55))
        return '7';
    if ((x >= 504 && y >= 193) && (x <= 504 + 112 && y <= 193 + 55))
        return '8';
    if ((x >= 635 && y >= 193) && (x <= 635 + 112 && y <= 193 + 55))
        return '9';
    if ((x >= 504 && y >= 273) && (x <= 504 + 112 && y <= 273 + 55))
        return '0';
    return '\0'; // 无效坐标
}

// 坐标清空为(-1,-1)
void CoordinateReset(void)
{
    ts_x = -1;
    ts_y = -1;
}
/***************************************END******************************************/
/***************************************数学处理******************************************/
// 生成大于 x 的随机数
int generateRandomValue(int x)
{
    int randomValue;

    // 确保生成的随机数在指定范围内并且大于 x
    do
    {
        randomValue = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
    } while (randomValue <= x);

    return randomValue;
}

// 生成大于等于 startnum 且小于等于 endnum 的随机整型数
int generateRandomInRange(int startnum, int endnum)
{
    // 确保 startnum 小于等于 endnum
    if (startnum > endnum)
    {
        fprintf(stderr, "错误: startnum 应小于等于 endnum\n");
        return -1; // 返回错误码
    }

    // 计算随机数范围
    int range = endnum - startnum + 1;

    // 生成并返回随机数
    return startnum + rand() % range;
}
// 判断当前时间是否为高峰期的函数
bool isPeakTime()
{
    // 获取当前时间
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    // 获取当前小时
    int hour = local->tm_hour;

    // 判断是否为高峰期
    if (hour >= 8 && hour < 22)
    {
        return true; // 高峰期
    }
    else
    {
        return false; // 非高峰期
    }
}
// 辅助函数：将秒数[seconds]转换为 "x小时x分钟x秒" 的格式 存放在[*buffer]中
void formatTimeTaken(int seconds, char *buffer, size_t bufferSize)
{
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    snprintf(buffer, bufferSize, "%d小时%d分钟%d秒", hours, minutes, secs);
}

// 将用户充电时长timeTaken 转换为 "时分秒" 格式的字符串 ,例如: formatTTaken(log->timeTaken, timeTakenBuffer, sizeof(timeTakenBuffer));
void formatTTaken(int timeTaken, char *timeTakenResult, size_t size)
{
    int hours = timeTaken / 3600;
    int minutes = (timeTaken % 3600) / 60;
    int seconds = timeTaken % 60;
    snprintf(timeTakenResult, size, "%02d:%02d:%02d", hours, minutes, seconds);
}
// 将用户消费日志时间consumption_at 转换为 "年月日 时分秒" 格式的字符串
void formatConsumptionTime(time_t consumption_at, char *consumptionResult, size_t size)
{
    struct tm *tm_info;
    tm_info = localtime(&consumption_at);                            // 装载时间戳,将时间转换为时间结构
    strftime(consumptionResult, size, "%Y-%m-%d %H:%M:%S", tm_info); // (消费时间)从时间结构中转换为我需要的格式 年月日 时分秒
}

/***************************************END******************************************/
/***************************************事件处理******************************************/
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
int isTouchButton(int ts_x, int ts_y, Button button)
{
    return (ts_x >= button.x && ts_y >= button.y &&
            ts_x <= button.x + button.width && ts_y <= button.y + button.height);
}
