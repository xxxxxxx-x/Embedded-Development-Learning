/***************************************头文件***************************************/
#include "./include/FileIOLibrary.h"
#include "./include/lcd.h"
#include "./include/charging.h"
#include "./include/touchpanel.h"
/***************************************END******************************************/

/**************************************全局变量**************************************/
extern LcdDev *lcd; // LCD的硬件信息的地址
extern char time_str[50];
extern int ts_fd;             // TS触摸屏的文件描述符
extern int ts_x, ts_y;        // 用于记录触摸屏的坐标
extern INTERFACE_STATE state; // 系统状态
extern bool refreshflag;      // 各个状态图像刷新
/***************************************END******************************************/

/**************************************程序入口**************************************/

int main(int argc, char const *argv[])
{
    /*********局部变量*********/
    int ts_x_old = ts_x; // 存放之前的触摸坐标
    int ts_y_old = ts_y; // 存放之前的触摸坐标
    /***END***/
    /*********设备初始化*********/
    // 1.硬件初始化
    lcd = lcd_init(LCD_PATH);                      // LCD的初始化
    ts_fd = ts_init(TS_PATH);                      // 触摸屏初始化
    clearScreen_LCD(lcd, getColor(0, 11, 21, 20)); // 刷新屏幕, 清除粤嵌标志
    /***END***/

    /*********开机动画*********/
    JPEG_ContinuousShow_LCD(0, 59, "../gif/", "Frame", 0, 0, lcd->mp, 800, 144);
    /***END***/

    /*********线程创建*********/
    pthread_t displaytime_lcd_thread; // 创建时间显示子线程
    pthread_create(&displaytime_lcd_thread, NULL, displaytime_lcd, NULL);
    pthread_t ts_thread; // 创建坐标获取子线程
    pthread_create(&ts_thread, NULL, ts_task, NULL);
    /***END***/

    /**************************************状态机**************************************/

    while (1)
    {

        switch (state)
        {
        case state_SelectEntry:
        {
            handle_SelectEntry();
        }
        break;
        case state_Authentication:
        {
            handle_Authentication();
        }
        break;
        case state_Login_Account:
        {
            handle_Login_Account();
        }
        break;
        case state_Login_Password:
        {
            handle_Login_Password();
        }
        break;
        case state_Registration_Account:
        {
            handle_Registration_Account();
        }
        break;
        case state_Registration_Password:
        {
            handle_Registration_Password();
        }
        break;
        case state_Registration_ConfirmPassword:
        {
            handle_Registration_ConfirmPassword();
        }
        break;
        case state_RegSuccess:
        {
            handle_RegSuccess();
        }
        break;
        case state_SystemMain:
        {
            handle_SystemMain();
        }
        break;
        case state_AccountEnquiry:
        {
            handle_AccountEnquiry();
        }
        break;
        case state_ChoosingChargingDev:
        {
            handle_ChoosingChargingDev();
        }
        break;
        case state_PriceList:
        {
            handle_PriceList();
        }
        break;
        case state_PromptCharger:
        {
            handle_PromptCharger();
        }
        break;
        case state_InCharge:
        {
            handle_InCharge();
        }
        break;
        case state_ChargingEndInfo:
        {
            handle_ChargingEndInfo();
        }
        break;
        case state_PatternPayment:
        {
            handle_PatternPayment();
        }
        break;
        case state_ScanPay:
        {
            handle_ScanPay();
        }
        break;
        case state_NotSufficientFunds:
        {
            handle_NotSufficientFunds();
        }
        break;
        case state_RechargeInterface:
        {
            handle_RechargeInterface();
        }
        break;
        case state_PaymentResult:
        {
            handle_PaymentResult();
        }
        break;
        default:
            break;
        }
        ts_x = -1;
        ts_y = -1;
    }

    /***************************************END******************************************/
    return 0;
}
