#pragma once

#include "types.h"
#include "platform.h"

typedef struct inf_rendImpl {
    void (*init)(void);
    void (*exit)(void);

    void (*PLAT_makeWindowRdata)(inf_window* window);
    void (*PLAT_destWindowRdata)(inf_window* window);
} inf_rendImpl;

extern const inf_rendImpl* inf_cur_rend_impl;

void inf_rend_useImpl(const inf_rendImpl* impl);

void inf_rend_init(void);
void inf_rend_exit(void);

void inf_rend_PLAT_makeWindowRdata(inf_window* window);
void inf_rend_PLAT_destWindowRdata(inf_window* window);
