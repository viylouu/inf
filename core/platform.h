#pragma once

#include "types.h"

typedef struct inf_windowDesc {
    u32 width;
    u32 height;
    const char* title;

    bool resizable;
    bool hidden;
} inf_windowDesc;

typedef struct inf_window {
    void* data;
    void* rdata;
    inf_windowDesc desc;
} inf_window;

typedef struct inf_platImpl {
    const char* api;

    void (*init)(void);
    void (*exit)(void); 

    bool (*poll)(inf_window* window);

    inf_window (*makeWindow)(inf_windowDesc desc);
    void (*destWindow)(inf_window* window);

    f64 (*getTime)(void);

    const char** (*VK_reqInstExts)(u32* count);

    void* (*XLIB_getWindow)(inf_window* window);
    void* (*XLIB_getDisplay)(void);
} inf_platImpl;

extern const inf_platImpl* inf_cur_plat_impl;

void inf_plat_useImpl(const inf_platImpl* impl);

void inf_plat_init(void);
void inf_plat_exit(void);

bool inf_plat_poll(inf_window* window);

inf_window inf_plat_makeWindow(inf_windowDesc desc);
void inf_plat_destWindow(inf_window* window);

f64 inf_plat_getTime(void);

const char** inf_plat_VK_reqInstExts(u32* count);

void* inf_plat_XLIB_getWindow(inf_window* window);
void* inf_plat_XLIB_getDisplay(void);
