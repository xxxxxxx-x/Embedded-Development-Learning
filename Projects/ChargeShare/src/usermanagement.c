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
#include "../include/usermanagement.h"
/**************************************全局变量**************************************/

/***************************************END******************************************/

/**************************************文件操作函数**************************************/
// 文件操作函数，包括存储、查询和修改用户信息
// 将用户信息写入用户信息文件中
/**
 * @name      writeUserInfo
 * @brief     将用户信息写入用户信息文件中
 * @param     user 用户信息结构体
 * @date      2024/05/18
 * @version   1.1
 * @note
 */
void writeUserInfo(UserInfo user)
{
    FILE *file = fopen(FILENAME, "a"); // 只写方式打开,不存在会创建新文件,文件偏移量移动到末尾
    if (file == NULL)
    {
        perror("打开用户信息文件失败");
        exit(EXIT_FAILURE); // 程序失败结束
    }
    fprintf(file, "%s,%s,%s,%.2f\n", user.username, user.password, user.created_at, user.balance); // 将用户信息写入文件
    fclose(file);
}
// 从用户信息文件中读取指定用户信息
/**
 * @name      readUserInfo
 * @brief     从用户信息文件中读取指定用户信息
 * @param     username 用户名称
 * @return
 *      @retval    [user]  成功读取到的用户信息结构体; [NULL] 用户不存在, 读取失败
 * @date      2024/05/18
 * @version   1.1
 * @note       注意!! 返回的是堆内存的地址用户储存区, 使用完要记得free()
 */
UserInfo *readUserInfo(const char *username)
{
    FILE *file = fopen(FILENAME, "r"); // 只读方式打开
    if (file == NULL)
    {
        FILE *filenew = fopen(FILENAME, "w+"); // 若打开失败,则文件不存在, 创建新文件
        fclose(filenew);
        perror("打开用户信息文件失败");
        return NULL;
    }
    UserInfo *user = (UserInfo *)malloc(sizeof(UserInfo)); // 用户信息 堆内存
    char line[200];
    while (fgets(line, sizeof(line), file)) // 从用户信息文件中读取一行
    {
        sscanf(line, "%[^,],%[^,],%[^,],%f", user->username, user->password, user->created_at, &user->balance); // 从字符串读取格式化输入
        /**
         * 格式说明
         * %[^,]：这是一个格式说明符，用于匹配任意字符，直到遇到逗号（,）为止。[^,] 表示一个字符集，^ 表示取反，即匹配除了逗号以外的所有字符。
         * %f：用于匹配一个浮点数。
         */
        if (strcmp(user->username, username) == 0) // 字符串比较, 相等返回0, 说明找到该用户信息
        {
            fclose(file);
            return user;
        }
    }
    fclose(file);
    free(user);
    return NULL;
}
// 更新用户信息到用户信息文件中
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
void updateUserInfo(UserInfo user)
{
    FILE *file = fopen(FILENAME, "r"); // 只读方式打开用户信息文件
    if (file == NULL)
    {
        perror("打开用户信息文件失败");
        exit(EXIT_FAILURE);
    }
    FILE *tempFile = fopen("../data/temp.txt", "w"); // 只写方式创建并打开临时文件
    if (tempFile == NULL)
    {
        perror("打开临时用户信息文件失败");
        exit(EXIT_FAILURE); // 程序失败结束
    }
    char line[200];                         // 读取一行数据的缓冲区
    UserInfo tempUser;                      // 用户信息结构体临时缓存区
    while (fgets(line, sizeof(line), file)) // 从用户信息文件中循环读取一行
    {
        sscanf(line, "%[^,],%[^,],%[^,],%f", tempUser.username, tempUser.password, tempUser.created_at, &tempUser.balance); // 将一行信息解析到用户信息结构缓存区
        if (strcmp(tempUser.username, user.username) == 0)                                                                  // 如果找到了该用户信息
        {
            fprintf(tempFile, "%s,%s,%s,%.2f\n", user.username, user.password, user.created_at, user.balance); // 将该用户信息的新信息写入到临时文件中
        }
        else
        {
            fprintf(tempFile, "%s", line); // 将没有该用户信息的部分也写入临时文件
        }
    }
    fclose(file);
    fclose(tempFile);
    remove(FILENAME);                    // 删除旧用户信息的文件
    rename("../data/temp.txt", FILENAME); // 将临时文件重命名为新文件名称
}
/***************************************END******************************************/
/**************************************文件操作函数**************************************/
// 用户登录和注册   未移植, 调用然后传入用户输入的数据即可
// 注册用户, 判断用户是否存在: 输入新用户信息, 然后写入到新文件中
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
int registerUser(char *username, char *password)
{

    UserInfo *user; // 用户信息结构体

    printf("你输入的用户名:%s 密码:%s\n", username, password);
    /*****查找  用户名  是否存在*****/
    user = readUserInfo(username); // 若成功读到用户信息
    if (user != NULL)
    {
        printf("用户已存在, 请登录\n");
        free(user);
        return 1;
    }
    UserInfo newUser;                   // 新用户信息结构体
    strcpy(newUser.username, username); // 更新名称
    strcpy(newUser.password, password); // 更新密码
    time_t now = time(NULL);            // 获取现在的系统时间  指向 struct tm 结构的指针，该结构包含分解的时间信息。
    // 格式化规则，格式化结构 timeptr 表示的时间，并把它存储在 str 中。
    /**
     * [存放新信息的字符数组] [接收的最大长度] [要格式化的格式] [系统产生的时间信息]
     * localtime 函数将 time_t 类型的时间（通常表示自1970年1月1日以来的秒数）
     *
     */
    strftime(newUser.created_at, sizeof(newUser.created_at), "%Y-%m-%d %H:%M:%S", localtime(&now));
    newUser.balance = REDPACKET; // 赠送红包赋值余额

    writeUserInfo(newUser); // 写入新文件
    printf("注册成功!信息已更新\n");
    return 0;
}
// 用户登录判断
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
int loginUser(char *username, char *password)
{
    UserInfo *user; // 用户信息结构体指针, 存放查找结果
    printf("你输入的用户名:%s 密码:%s\n", username, password);

    /*****查找  用户名  是否存在*****/
    user = readUserInfo(username); /// 读到则为用户信息, 反之则为null
    if (user == NULL)
    {
        printf("用户不存在!\n");
        return 1;
    }
    /*****查找密码是否正确*****/
    // 核对密码
    if (strcmp(user->password, password) == 0)
    {
        printf("登录成功(账号密码正确)!\n");
        return 0;
    }
    else
    {
        printf("账号存在, 但密码错误\n");
        return 2;
    }
    free(user);
}

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
void addUserCashlog(UserCashlog *DestUeser_log)
{
    char filename[100];                                                     // 存放目标用户的日志文件名称
    createLogFileName(filename, sizeof(filename), DestUeser_log->username); // 生成目标用户的日志文件名称

    FILE *file = fopen(filename, "a"); // 只写, 不存在则创建, 定位到末尾
    if (file == NULL)
    {
        perror("打开用户消费日志文件失败");
        exit(EXIT_FAILURE); // 程序错误退出
    }

    fprintf(file, "%ld,%.2f,%.2f,%s,%s,%d,%.2f,%s\n",
            DestUeser_log->consumption_at,      // 用户消费时间(秒)
            DestUeser_log->sum,                 // 用户单笔订单费用
            DestUeser_log->balance,             // 用户余额
            DestUeser_log->chargingStationName, // 充电站名称
            DestUeser_log->model,               // 选择的充电设备型号
            DestUeser_log->timeTaken,           // 冲了多少时间(秒)
            DestUeser_log->totalElectricity,    // 消耗了多少电(度)
            DestUeser_log->remark);             // 备注 例如充值或消费
    fclose(file);
}

// 创建日志文件名
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
void createLogFileName(char *buffer, size_t bufferSize, const char *username)
{
    printf("进入创建日志文件名模块, 取到的用户名称是%s\n", username);

    snprintf(buffer, bufferSize, "../data/%s_log.txt", username);
    printf("生成的含路径的用户名称是%s\n", buffer);
}

// 获取用户消费日志数量
/**
 * @name      getUserCashlogCount
 * @brief     获取用户消费日志数量, 去对应文件中查找
 * @param     username 用户名称
 * @return
 *      @retval [count]用户日志记录条数; [1]失败
 * @date      2024/05/20
 * @version   1.0
 * @note
 */
int getUserCashlogCount(const char *username)
{
    printf("进入获取用户消费日志数量模块\n取到的用户名称是%s", username);
    char filename[100];                                      // 存放目标用户的日志文件名称
    createLogFileName(filename, sizeof(filename), username); // 生成目标用户的日志文件名称

    FILE *file = fopen(filename, "r"); // 只读打开
    if (file == NULL)
    {
        perror("打开用户消费日志文件失败, 文件可能不存在, 请尝试创建用户或产生日志修复o(╥﹏╥)o");
        return 1;
    }

    int count = 0;
    char line[256];                         // 读取一行数据的缓冲区, 遇到换行符号\n停止
    while (fgets(line, sizeof(line), file)) // 当读到一行
    {
        count++;
    }
    fclose(file);
    return count;
}

// 得到用户的第count条消费日志
/**
 * @name      queryUserCashlog
 * @brief     显示用户的第count条消费日志
 * @param     username 要查找的目标用户名称
 * @param     desCount 第count条消费日志 从0开始计算
 * @param     log      接收查找到的消费日志结构体信息
 * @return    0 成功，1 失败
 * @date      2024/05/20
 * @version   1.0
 * @note      展示没有想到更好的方式, 一次性停顿逐行显示的话, 就需要中断或线程, 比较麻烦
 *            可以先调用计算函数计算有多少行
 */
int queryUserCashlog(const char *username, int desCount, UserCashlog *log)
{
    char filename[100];                                      // 存放目标用户的日志文件名称
    createLogFileName(filename, sizeof(filename), username); // 生成目标用户的日志文件名称

    FILE *file = fopen(filename, "r"); // 只读打开
    if (file == NULL)
    {
        perror("打开用户消费日志文件失败, 文件可能不存在, 请尝试创建用户或产生日志修复o(╥﹏╥)o");
        return 1;
    }
    int count = 0;                          // 计数器, 第x行了
    char line[256];                         // 读取一行数据的缓冲区, 遇到换行符号\n停止
    while (fgets(line, sizeof(line), file)) // 逐行判断
    {
        if (count == desCount)
        {
            // 从字符串line(读取结果缓存)读取格式化输入到缓存结构体中
            sscanf(line, "%ld,%f,%f,%[^,],%[^,],%d,%f,%[^,]",
                   &log->consumption_at,     // 用户消费时间(秒)
                   &log->sum,                // 用户单笔订单费用
                   &log->balance,            // 用户余额
                   log->chargingStationName, // 充电站名称
                   log->model,               // 选择的充电设备型号
                   &log->timeTaken,          // 冲了多少时间(秒)
                   &log->totalElectricity,   // 消耗了多少电(度)
                   log->remark);             // 备注 例如充值或消费

            strcpy(log->username, username); // 用于日志中没有用户名称, 故而这里更新名称
            fclose(file);
            return 0;
        }
        count++;
    }
    fclose(file);
    printf("没有找到第%d条消费日志\n", desCount);
    return 1;
}

// 在控制台上打印用户消费记录-->调试用
void printUserCashlog(UserCashlog *log)
{
    // 定义用于存储时间字符串的字符数组
    // 接收消费时间的转换结果
    char timeBuffer[50];
    // 接收充电时间的转换结果
    char timeTakenBuffer[50];
    // 定义一个 struct tm 指针
    struct tm *tm_info;
    // 将 time_t 类型的时间转换为 struct tm 结构
    tm_info = localtime(&log->consumption_at); // 装载时间戳,将时间转换为时间结构
    // 将 struct tm 结构中的时间信息格式化为字符串
    strftime(timeBuffer, 50, "%Y-%m-%d %H:%M:%S", tm_info); // (消费时间)从时间结构中转换为我需要的格式 年月日 时分秒

    formatTimeTaken(log->timeTaken, timeTakenBuffer, sizeof(timeTakenBuffer)); // 转化充电时间为时分秒

    printf("用户名: %s\n", log->username);
    printf("消费时间: %s\n", timeBuffer);
    printf("消费金额: %.2f\n", log->sum);
    printf("用户余额: %.2f\n", log->balance);
    printf("充电站名称: %s\n", log->chargingStationName);
    printf("设备型号: %s\n", log->model);
    printf("一次用时: %s\n", timeTakenBuffer);
    printf("耗电量: %.2f度\n", log->totalElectricity);
    printf("备注: %s\n", log->remark);
}

// 删除目标用户的所有消费日志
/**
 * @name      queryUserCashlog
 * @brief     删除目标用户的所有消费日志
 * @param     username 要查找的目标用户名称
 * @return    0 成功，1 失败
 * @date      2024/05/20
 * @version   1.0
 * @note
 */
void deleteUserCashlog(const char *username)
{
    char filename[100];
    createLogFileName(filename, sizeof(filename), username); // 合成日志名称

    FILE *file = fopen(filename, "w"); // 不存在则创建, 存在则清空
    if (file == NULL)
    {
        perror("打开用户消费日志文件失败");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}