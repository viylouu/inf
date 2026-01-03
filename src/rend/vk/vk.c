#include "vk.h"
#include "loc.h"

#include "instance.c"
#include "device.c"
#include "surface.c"
#include "swapchain.c"
#include "imageviews.c"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

static void vk_init(inf_window* window) {
    inf_debug_msg("initializing vulkan...");

    // cannot call muliple times! change to use window array for init

    _vk_createInstance();

    _vk_createSurface(window);

    _vk_pickPhysicalDevice();
    _vk_createLogicalDevice();

    _vk_createSwapchain(window);

    _vk_createImageViews();

    inf_debug_msg("initialized vulkan!");
}
static void vk_exit(void) {
    inf_debug_msg("exiting vulkan...");

    _vk_deleteImageViews();
    
    _vk_deleteSwapchain();

    _vk_deleteDevice();

    _vk_deleteSurface();

    _vk_deleteInstance();

    inf_debug_msg("exited vulkan!");
}

const inf_rendImpl inf_vk_impl = (inf_rendImpl){
        .init = vk_init,
        .exit = vk_exit,
    };
