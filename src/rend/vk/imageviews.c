// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <X11/X.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

static void _vk_createImageViews(inf_window* window) {
    inf_debug_msg("creating image views...");

    vk_windowRdata* rd = window->rdata;

    rd->scimgviewamt = rd->scimgamt;
    rd->swapchainimgviews = inf_malloc(sizeof(VkImageView) * rd->scimgviewamt);

    for (u32 i = 0; i < rd->scimgviewamt; ++i) {
        VkImageViewCreateInfo createinfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = NULL,

                .flags = 0,

                .image = rd->swapchainimgs[i],

                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = rd->swapchainimgfmt,

                .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,

                .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .subresourceRange.baseMipLevel = 0,
                .subresourceRange.levelCount = 1,
                .subresourceRange.baseArrayLayer = 0,
                .subresourceRange.layerCount = 1,
            };

        if (vkCreateImageView(s.device, &createinfo, NULL, &rd->swapchainimgviews[i]) != VK_SUCCESS) {
            inf_err_msg("failed to create image views!");
            exit(1);
        }
    }

    inf_debug_msg("created image views!");
}

static void _vk_deleteImageViews(inf_window* window) {
    vk_windowRdata* rd = window->rdata;

    for (u32 i = 0; i < rd->scimgviewamt; ++i)
        vkDestroyImageView(s.device, rd->swapchainimgviews[i], NULL);

    inf_free(rd->swapchainimgviews);

    inf_debug_msg("deleted image views!");
}
