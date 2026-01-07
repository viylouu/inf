#include "vk.h"
//#include "loc.h"

#include "instance.c"
#include "device.c"
#include "loc.h"
#include "surface.c"
#include "swapchain.c"
#include "imageviews.c"
#include "shader.c"
#include "pipeline.c"
#include "rendpass.c"
#include "framebuffer.c"

static void vk_init(void) {
    inf_debug_msg("initializing vulkan...");

    // cannot call muliple times! change to use window array for init

    _vk_createInstance();

    s.tempwindow = inf_cur_plat_impl->makeWindow((inf_windowDesc){
                .width = 1,
                .height = 1,
                .resizable = false,
                .hidden = true,
                .title = "vulkan hidden 1x1 lego piece ahh window",
            });
    s.twr = s.tempwindow.rdata = inf_malloc(sizeof(vk_windowRdata));

    _vk_createSurface(&s.tempwindow);

    _vk_pickPhysicalDevice();
    _vk_createLogicalDevice();

    inf_debug_msg("initialized vulkan!");
}
static void vk_exit(void) {
    inf_debug_msg("exiting vulkan...");

    _vk_deleteDevice();

    _vk_deleteSurface(&s.tempwindow);
    inf_free(s.tempwindow.rdata);
    inf_cur_plat_impl->destWindow(&s.tempwindow);

    _vk_deleteInstance();

    inf_debug_msg("exited vulkan!");
}

static inf_shader vk_makeShader(inf_shaderDesc desc) {
    inf_debug_msg("making shader...");

    inf_shader shader = (inf_shader){
            .data = inf_malloc(sizeof(vk_shaderData)),
            .desc = desc,
        };
    vk_shaderData* sd = shader.data;

    sd->module = _vk_createShaderModule(desc.source, desc.sourcesize);

    inf_debug_msg("made shader!");
    return shader;
}
static void vk_destShader(inf_shader* shader) {
    inf_debug_msg("deleting shader...");

    vk_shaderData* sd = shader->data;
    _vk_deleteShaderModule(sd->module);

    inf_free(shader->data);

    inf_debug_msg("deleted shader!");
}

static inf_pipeline vk_makePipeline(inf_pipelineDesc desc) {
    inf_debug_msg("making pipeline...");

    inf_pipeline pipeline = (inf_pipeline){
            .data = inf_malloc(sizeof(vk_pipelineData)),
            .desc = desc,
        };

    _vk_createPipeline(&pipeline);

    inf_debug_msg("made pipeline!");
    return pipeline;
}
static void vk_destPipeline(inf_pipeline* pipeline) {
    inf_debug_msg("deleting pipeline...");

    _vk_deletePipeline(pipeline);
    inf_free(pipeline->data);

    inf_debug_msg("deleted pipeline!");
}

static void vk_PLAT_makeWindowRdata(inf_window* window) {
    inf_debug_msg("making window rdata...");
    
    window->rdata = inf_malloc(sizeof(vk_windowRdata));

    _vk_createSurface(window);
    _vk_createSwapchain(window);
    _vk_createImageViews(window);
    _vk_createRenderPass(window);
    _vk_createFramebuffers(window);

    inf_debug_msg("made window rdata!");
}
static void vk_PLAT_destWindowRdata(inf_window* window) {
    inf_debug_msg("deleting window rdata...");

    _vk_deleteFramebuffers(window);
    _vk_deleteRenderPass(window);
    _vk_deleteImageViews(window);
    _vk_deleteSwapchain(window);
    _vk_deleteSurface(window);

    inf_free(window->rdata);

    inf_debug_msg("deleted window rdata!");
}

const inf_rendImpl inf_vk_impl = (inf_rendImpl){
        .init = vk_init,
        .exit = vk_exit,

        .makeShader = vk_makeShader,
        .destShader = vk_destShader,

        .makePipeline = vk_makePipeline,
        .destPipeline = vk_destPipeline,

        .PLAT_makeWindowRdata = vk_PLAT_makeWindowRdata,
        .PLAT_destWindowRdata = vk_PLAT_destWindowRdata,
    };
