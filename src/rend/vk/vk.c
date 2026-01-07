#include "vk.h"
//#include "loc.h"

#include "instance.c"
#include "loc.h"
#include "surface.c"
#include "swapchain.c"
#include "imageviews.c"
#include "shader.c"
#include "pipeline.c"
#include "rendpass.c"
#include "framebuffer.c"
#include "commandpool.c"
#include "device.c"
#include "commandbuffer.c"
#include "sync.c"

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

    _vk_createCommandPool();
    _vk_createCommandBuffers();

    _vk_createSyncObjects();

    inf_debug_msg("initialized vulkan!");
}
static void vk_exit(void) {
    inf_debug_msg("exiting vulkan...");

    _vk_deleteSyncObjects();

    _vk_deleteCommandBuffers();
    _vk_deleteCommandPool();

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

static void vk_frameStart(inf_window* window, f32 clear[4]) {
    inf_debug_msg("starting frame...");

    vk_windowRdata* rd = window->rdata;

    _vk_waitForFences(rd->curframe);
    rd->curimgindex = _vk_acquireSwapchainImage(window, rd->curframe);

    _vk_resetCommandBuffer(s.cmdbuffers[rd->curframe]);
    _vk_startCommandBuffer(s.cmdbuffers[rd->curframe], rd->curimgindex);
    _vk_startRenderPass(window, rd->curimgindex, clear, rd->curframe);

    inf_debug_msg("started frame!");
}
static void vk_frameEnd(inf_window* window) {
    inf_debug_msg("ending frame...");

    vk_windowRdata* rd = window->rdata;

    _vk_endRenderPass(rd->curframe);
    _vk_endCommandBuffer(s.cmdbuffers[rd->curframe]);

    _vk_submitCommandBuffer(&s.cmdbuffers[rd->curframe], rd->curframe);
    _vk_presentSwapchain(window, rd->curimgindex, rd->curframe);

    ++rd->curframe;
    if (rd->curframe >= INF_VK_MAX_FRAMES_IN_FLIGHT)
        rd->curframe = 0;

    inf_debug_msg("ended frame!");
}

static void vk_PLAT_makeWindowRdata(inf_window* window) {
    inf_debug_msg("making window rdata...");
    
    window->rdata = inf_malloc(sizeof(vk_windowRdata));

    vk_windowRdata* rd = window->rdata;
    rd->curframe = 0;
    rd->curimgindex = 0;

    _vk_createSurface(window);
    _vk_createSwapchain(window);
    _vk_createImageViews(window);
    _vk_createRenderPass(window);
    _vk_createFramebuffers(window);

    inf_debug_msg("made window rdata!");
}
static void vk_PLAT_destWindowRdata(inf_window* window) {
    inf_debug_msg("deleting window rdata...");

    _vk_deviceWaitIdle();

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

        .frameStart = vk_frameStart,
        .frameEnd = vk_frameEnd,

        .PLAT_makeWindowRdata = vk_PLAT_makeWindowRdata,
        .PLAT_destWindowRdata = vk_PLAT_destWindowRdata,
    };
