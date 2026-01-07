// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

//#include <string.h>

static void _vk_createSyncObjects(void) {
    inf_debug_msg("creating sync objects...");
    
    VkSemaphoreCreateInfo semaphoreinfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,
        };

    VkFenceCreateInfo fenceinfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = NULL,

            .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };

    for (u32 i = 0; i < INF_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
        if (vkCreateSemaphore(s.device, &semaphoreinfo, NULL, &s.imgavailablesems[i]) != VK_SUCCESS
         || vkCreateSemaphore(s.device, &semaphoreinfo, NULL, &s.renderfinsems[i]) != VK_SUCCESS
         || vkCreateFence(s.device, &fenceinfo, NULL, &s.inflightfences[i]) != VK_SUCCESS) {
            inf_err_msg("failed to create sync objects for a frame!");
            exit(1);
        }
    }

    inf_debug_msg("created sync objects!");
}

static void _vk_deleteSyncObjects(void) {
    inf_debug_msg("deleting sync objects...");

    for (u32 i =0; i < INF_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroyFence(s.device, s.inflightfences[i], NULL);
        vkDestroySemaphore(s.device, s.renderfinsems[i], NULL);
        vkDestroySemaphore(s.device, s.imgavailablesems[i], NULL);
    }

    inf_debug_msg("deleted sync objects!");
}


static void _vk_waitForFences(u32 frame) {
    inf_debug_msg("waiting for fences...");
    vkWaitForFences(s.device, 1, &s.inflightfences[frame], VK_TRUE, 0);
    vkResetFences(s.device, 1, &s.inflightfences[frame]);
    inf_debug_msg("waited for fences!");
}
