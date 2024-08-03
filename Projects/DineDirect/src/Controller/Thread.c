#include "../../include/Controller/Thread.h"

void *lvgl_task_handler(void *arg) {
    while (1) {
        lv_tick_inc(5);
        lv_timer_handler(); // 处理LVGL任务
        usleep(5000);
    }
}

bool lvgl_thread(void) {
    lv_thread_t lv_thread_t;
    if (NULL == lv_thread_t) {
        return false;
    }
    pthread_create(&lv_thread_t, NULL, lvgl_task_handler, NULL);
    return true;
}
