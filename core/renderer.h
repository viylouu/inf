#pragma once

#include "types.h"
#include "platform.h"

typedef struct inf_rendImpl {
    void (*init)(inf_window* window);
    void (*exit)(void);
} inf_rendImpl;

extern const inf_rendImpl* inf_cur_rend_impl;

void inf_rend_useImpl(const inf_rendImpl* impl);

void inf_rend_init(inf_window* window);
void inf_rend_exit(void);
