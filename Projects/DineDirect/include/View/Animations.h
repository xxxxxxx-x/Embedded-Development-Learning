/****************************************************************************************************************************************************************
 * @file         : /DineDirect/include/View/Animations.h
 * @brief        : 
 * @author       : a_23456@foxmail.com
 * @date         : 2024-06-20 15:51:41
 * @version      : 1.0
 * @note         : 
 * @Copyright    : (c)   2023-2024   a_23456@foxmail.com   All Right Reserved
*****************************************************************************************************************************************************************/

#include <stdio.h>
#include "../../lvgl/lvgl.h"
#include "../../lvgl/widgets/image/lv_image.h"
#include "../../lvgl/widgets/spinner/lv_spinner.h"
#include "font_alimama_bold_60.h"

bool loading_animation();
void destroy_animation(lv_timer_t * timer) ;
