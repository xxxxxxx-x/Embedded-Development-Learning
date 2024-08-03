/***************************************头文件***************************************/
#include "../../include/Controller/Thread.h"
#include "../../include/Controller/Controller.h"
#include "../../include/View/Animations.h"

/**************************************全局变量**************************************/
typedef enum {
    STATE_TIMER,
    STATE_INIT,
    STATE_LOADING_ANIMATIONS,
    STATE_MAIN,
    STATE_ERROR,
    STATE_SHUTDOWN
} SYSTEM_STATE;

SYSTEM_STATE system_state = STATE_TIMER; // 系统初始状态为初始化

/**************************************程序入口**************************************/

int main(int argc, char const *argv[]) {

    /**************************************状态机**************************************/
    while (1) {
        switch (system_state) {
            case STATE_TIMER:
                //初始化定时器
                system_state = lvgl_thread() ? STATE_INIT : STATE_ERROR;
                break;
            case STATE_INIT:
                // 执行初始化状态的逻辑
                system_state = initialize_x11() ? STATE_LOADING_ANIMATIONS : STATE_ERROR;
                break;
            case STATE_LOADING_ANIMATIONS:
                //加载开机动画
                system_state = loading_animation() ? STATE_MAIN : STATE_ERROR;
                break;
            case STATE_MAIN:
                printf("正在进入菜单页...\n");

                break;
            case STATE_ERROR:
                // 执行错误状态的逻辑
                printf("系统遇到错误...\n");
                // 可能执行错误处理、重启等操作
                system_state = STATE_SHUTDOWN; // 例子中直接切换到关机状态
                break;

            case STATE_SHUTDOWN:
                // 执行关机状态的逻辑
                printf("系统正在关闭...\n");
                // 执行关机动作
                return 0; // 结束程序
        }
    }
    /***************************************END******************************************/
}
