#pragma once

#include "vk.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

static struct state {
    VkInstance instance;
    VkPhysicalDevice physicaldevice;
    VkDevice device;
    VkQueue graphicsqueue;
    VkQueue presentqueue;

    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkImage* swapchainimgs;
        u32 scimgamt;
    VkFormat swapchainimgfmt;
    VkExtent2D swapchainext;
} s;

static const char* validations[] = {
    "VK_LAYER_KHRONOS_validation",
};

#ifdef INF_DEBUG
static const bool validation = true;
#else
static const bool validation = false;
#endif

static const char* devexts[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};
static const u32 devextamt = 1;
