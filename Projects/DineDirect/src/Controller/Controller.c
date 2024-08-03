#include "../../include/Controller/Controller.h"

extern const lv_font_t lv_font_montserrat_48;

/****************************************************************************************************************************************************************
 * @name: initialize_x11
 * @brief: 初始化X11显示
 * @return 成功返回true，否则返回false
 * @date: 2024-06-20 16:54:32
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
bool initialize_x11(void) {
    printf("系统正在初始化x11...\n");
    lv_init();
    // 初始化 X11 显示驱动（假设 monitor_hor_res 和 monitor_ver_res 是已定义的分辨率变量）
    lv_display_t *disp = lv_x11_window_create("DineDirect", 800, 480);
    if (disp == NULL) {
        printf("X11 显示设备创建失败\n");
        return false;
    }

    printf("系统正在初始化鼠标样式...\n");
    // 初始化 X11 输入驱动（键盘、鼠标和鼠标滚轮）
    LV_IMG_DECLARE(mouse_cursor_icon);
    lv_x11_inputs_create(disp, &mouse_cursor_icon);

    return true;
}

