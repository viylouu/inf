// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

VkShaderModule _vk_createShaderModule(const char* code, u32 amt) {
    VkShaderModuleCreateInfo createinfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .codeSize = amt,
            .pCode = (const u32*)code, // dunno why its u32* but ok
        };

    VkShaderModule mod;
    if (vkCreateShaderModule(s.device, &createinfo, NULL, &mod) != VK_SUCCESS) {
        inf_warn_msg("failed to create shader module! returning 0!");
        return 0;
    }

    inf_debug_msg("created shader module!");

    return mod;
}

void _vk_deleteShaderModule(VkShaderModule mod) {
    vkDestroyShaderModule(s.device, mod, NULL);
    inf_debug_msg("deleted shader module!");
}
