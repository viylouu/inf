#pragma once

#include "vk.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct vk_windowRdata {
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkImage* swapchainimgs;
        u32 scimgamt;
    VkFormat swapchainimgfmt;
    VkExtent2D swapchainext;
    VkImageView* swapchainimgviews;
        u32 scimgviewamt;
    VkFramebuffer* swapchainfbufs;
        u32 scfbufamt;

    VkRenderPass renderpass;

    u32 curimgindex;
} vk_windowRdata;

typedef struct vk_shaderData {
    VkShaderModule module;
} vk_shaderData;

typedef struct vk_pipelineData {
    VkPipelineLayout layout;
} vk_pipelineData;

static struct state {
    VkInstance instance;
    VkPhysicalDevice physicaldevice;
    VkDevice device;
    VkQueue graphicsqueue;
    VkQueue presentqueue;
    VkCommandPool cmdpool;
    VkCommandBuffer cmdbuffer;

    VkSemaphore imgavailablesem;
    VkSemaphore renderfinsem;
    VkFence inflightfence;

    // basically only for device creation
    inf_window tempwindow;
    vk_windowRdata* twr; // tempwindow rdata
} s;

static const char* validations[] = {
    "VK_LAYER_KHRONOS_validation",
};
static const u32 validamt = 1;

#ifdef INF_DEBUG
static const bool validation = true;
#else
static const bool validation = false;
#endif

static const char* devexts[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};
static const u32 devextamt = 1;

// no clue why its famine but ok
typedef struct _queueFamine {
    u32 graphicsfam;
    bool hasgraphicsfam;

    u32 presentfam;
    bool haspresentfam;

    bool complete;
} _queueFamine;

static _queueFamine _vk_findQueueFamilies(VkPhysicalDevice device);
