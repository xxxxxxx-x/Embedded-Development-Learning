//
// Created by root_ on 24-6-29.
//

#ifndef DINEDIRECT_THREAD_H
#define DINEDIRECT_THREAD_H

#include <unistd.h>
#include "../../lvgl/lvgl.h"

void *lvgl_task_handler(void *arg);
bool lvgl_thread(void);

#endif //DINEDIRECT_THREAD_H
