#include "../../include/View/Animations.h"

bool loading_animation() {
    printf("系统正在加载开机动画...\n");
    LV_FONT_DECLARE(font_alimama_bold_60);
    lv_obj_t *spinner = lv_spinner_create(lv_screen_active());
    lv_obj_set_size(spinner, 100, 100);
    lv_obj_center(spinner);
    lv_spinner_set_anim_params(spinner, 5000, 200);

    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x212745), LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "DineDirect智点餐");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_font(label,&font_alimama_bold_60,LV_PART_MAIN);
    lv_obj_set_style_text_letter_space(lv_screen_active(),10,LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -100);
    lv_timer_create(destroy_animation, 15000, spinner);
    lv_timer_create(destroy_animation, 15000, label);

}

void destroy_animation(lv_timer_t *timer) {
    lv_obj_t *lv_obj = (lv_obj_t *) timer->user_data;
    lv_obj_del(lv_obj);  // 销毁 spinner 对象
}