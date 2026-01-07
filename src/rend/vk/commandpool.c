// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

//#include <string.h>

static void _vk_createCommandPool(void) {
    inf_debug_msg("creating command pool...");

    _queueFamine famine = _vk_findQueueFamilies(s.physicaldevice);

    VkCommandPoolCreateInfo poolinfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = NULL,

            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // FINALLY

            .queueFamilyIndex = famine.graphicsfam,
        };

    if (vkCreateCommandPool(s.device, &poolinfo, NULL, &s.cmdpool) != VK_SUCCESS) {
        inf_err_msg("failed to create command pool!");
        exit(1);
    }

    inf_debug_msg("created command pool!");
}

static void _vk_deleteCommandPool(void) {
    inf_debug_msg("deleting command pool...");

    vkDestroyCommandPool(s.device, s.cmdpool, NULL);

    inf_debug_msg("deleted command pool!");
}
