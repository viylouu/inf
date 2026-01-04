#include "../../main.h"

const inf_rendImpl* inf_cur_rend_impl;

void inf_rend_useImpl(const inf_rendImpl* impl) {
    inf_cur_rend_impl = impl;
}

void inf_rend_init(void) {
    inf_cur_rend_impl->init();
}
void inf_rend_exit(void) {
    inf_cur_rend_impl->exit();
}

inf_shader inf_rend_makeShader(inf_shaderDesc desc) {
    return inf_cur_rend_impl->makeShader(desc);
}
void inf_rend_destShader(inf_shader* shader) {
    inf_cur_rend_impl->destShader(shader);
}

void inf_rend_PLAT_makeWindowRdata(inf_window* window) {
    inf_cur_rend_impl->PLAT_makeWindowRdata(window);
}
void inf_rend_PLAT_destWindowRdata(inf_window* window) {
    inf_cur_rend_impl->PLAT_destWindowRdata(window);
}
