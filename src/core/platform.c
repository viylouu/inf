#include "../../main.h"

const inf_platImpl* inf_cur_plat_impl;

void inf_plat_useImpl(const inf_platImpl* impl) {
    inf_cur_plat_impl = impl;
}

void inf_plat_init(void) {
    inf_cur_plat_impl->init();
}
void inf_plat_exit(void) {
    inf_cur_plat_impl->exit();
}

bool inf_plat_poll(inf_window* window) {
    return inf_cur_plat_impl->poll(window);
}

inf_window inf_plat_makeWindow(inf_windowDesc desc) {
    return inf_cur_plat_impl->makeWindow(desc);
}
void inf_plat_destWindow(inf_window* window) {
    inf_cur_plat_impl->destWindow(window);
}

f64 inf_plat_getTime(void) {
    return inf_cur_plat_impl->getTime();
}

const char** inf_plat_VK_reqInstExts(u32* count) {
    return inf_cur_plat_impl->VK_reqInstExts(count);
}

void* inf_plat_XLIB_getWindow(inf_window* window) {
    return inf_cur_plat_impl->XLIB_getWindow(window);
}
void* inf_plat_XLIB_getDisplay(void) {
    return inf_cur_plat_impl->XLIB_getDisplay();
}
