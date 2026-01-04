#include "../../main.h"

#include <string.h>

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

// reason these call the rdata funcs is for renderer to make a window by calling the inf_cur_plat_impl ver instead of the helper funcs to bypass calling its own make rdata functions so it doesent do that before it creates the logical device for vulkan by creating a temp window
// ^^^ thats alot of text
inf_window inf_plat_makeWindow(inf_windowDesc desc) {
    inf_window w = inf_cur_plat_impl->makeWindow(desc);
    inf_rend_PLAT_makeWindowRdata(&w);
    return w;
}
void inf_plat_destWindow(inf_window* window) {
    inf_rend_PLAT_destWindowRdata(window);
    inf_cur_plat_impl->destWindow(window);
}

f64 inf_plat_getTime(void) {
    return inf_cur_plat_impl->getTime();
}

const char** inf_plat_VK_reqInstExts(u32* count) {
    return inf_cur_plat_impl->VK_reqInstExts(count);
}

void* inf_plat_XLIB_getWindow(inf_window* window) {
    if (!strcmp(inf_cur_plat_impl->api, "xlib"))
        return inf_cur_plat_impl->XLIB_getWindow(window);
    else {
        inf_warn_msg("tried to call xlib function on non xlib plat! returning null! (XLIB_getWindow)");
        return NULL;
    }
}
void* inf_plat_XLIB_getDisplay(void) {
    if (!strcmp(inf_cur_plat_impl->api, "xlib"))
        return inf_cur_plat_impl->XLIB_getDisplay();
    else {
        inf_warn_msg("tried to call xlib function on non xlib plat! returning null! (XLIB_getDisplay)");
        return NULL;
    }
}
