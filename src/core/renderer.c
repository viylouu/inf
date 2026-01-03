#include "../../main.h"

const inf_rendImpl* inf_cur_rend_impl;

void inf_rend_useImpl(const inf_rendImpl* impl) {
    inf_cur_rend_impl = impl;
}

void inf_rend_init(inf_window* window) {
    inf_cur_rend_impl->init(window);
}
void inf_rend_exit(void) {
    inf_cur_rend_impl->exit();
}
