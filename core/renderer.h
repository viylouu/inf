#pragma once

#include "types.h"
#include "platform.h"

typedef struct inf_shaderDesc {
    char* source;
    u32 sourcesize;
} inf_shaderDesc;

typedef struct inf_shader {
    void* data;
    inf_shaderDesc desc;
} inf_shader;

typedef struct inf_pipelineDesc {
    inf_shader* vertex;
    inf_shader* fragment;
} inf_pipelineDesc;

typedef struct inf_pipeline {
    void* data;
    inf_pipelineDesc desc;
} inf_pipeline;

typedef struct inf_rendImpl {
    void (*init)(void);
    void (*exit)(void);

    inf_shader (*makeShader)(inf_shaderDesc desc);
    void (*destShader)(inf_shader* shader);

    inf_pipeline (*makePipeline)(inf_pipelineDesc desc);
    void (*destPipeline)(inf_pipeline* pipeline);

    void (*PLAT_makeWindowRdata)(inf_window* window);
    void (*PLAT_destWindowRdata)(inf_window* window);
} inf_rendImpl;

extern const inf_rendImpl* inf_cur_rend_impl;

void inf_rend_useImpl(const inf_rendImpl* impl);

void inf_rend_init(void);
void inf_rend_exit(void);

inf_shader inf_rend_makeShader(inf_shaderDesc desc);
void inf_rend_destShader(inf_shader* shader);

inf_pipeline inf_rend_makePipeline(inf_pipelineDesc desc);
void inf_rend_destPipeline(inf_pipeline* pipeline);

void inf_rend_PLAT_makeWindowRdata(inf_window* window);
void inf_rend_PLAT_destWindowRdata(inf_window* window);
