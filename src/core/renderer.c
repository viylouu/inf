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

inf_pipeline inf_rend_makePipeline(inf_pipelineDesc desc) {
    return inf_cur_rend_impl->makePipeline(desc);
}
void inf_rend_destPipeline(inf_pipeline* pipeline) {
    inf_cur_rend_impl->destPipeline(pipeline);
}

void inf_rend_frameStart(inf_window* window, f32 clear[4]) {
    inf_cur_rend_impl->frameStart(window, clear);
}
void inf_rend_frameEnd(inf_window* window) {
    inf_cur_rend_impl->frameEnd(window);
}

void inf_rend_PLAT_makeWindowRdata(inf_window* window) {
    inf_cur_rend_impl->PLAT_makeWindowRdata(window);
}
void inf_rend_PLAT_destWindowRdata(inf_window* window) {
    inf_cur_rend_impl->PLAT_destWindowRdata(window);
}
