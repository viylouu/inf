// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

//#include <string.h>

static void _vk_createCommandBuffer(void) {
    inf_debug_msg("creating command buffer...");

    VkCommandBufferAllocateInfo allocinfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = NULL,

            .commandPool = s.cmdpool,
            
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,

            .commandBufferCount = 1,
        };

    if (vkAllocateCommandBuffers(s.device, &allocinfo, &s.cmdbuffer) != VK_SUCCESS) {
        inf_err_msg("failed to create command buffer!");
        exit(1);
    }

    inf_debug_msg("created command buffer!");
}

static void _vk_deleteCommandBuffer(void) {
    // apparently isnt needed?

    //inf_debug_msg("deleting command buffer...");

    //vkFreeCommandBuffer(s.device, s.cmdbuffer, NULL);

    //inf_debug_msg("deleted command buffer!");

    inf_debug_msg("note: command buffer does not need to be freed!");
}


static void _vk_resetCommandBuffer(VkCommandBuffer buffer) {
    vkResetCommandBuffer(buffer, 0); 
    inf_debug_msg("reset command buffer!");
}

static void _vk_startCommandBuffer(VkCommandBuffer buffer, u32 index) {
    VkCommandBufferBeginInfo begininfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = NULL,

            .flags = 0,

            .pInheritanceInfo = NULL,
        };

    if (vkBeginCommandBuffer(buffer, &begininfo) != VK_SUCCESS) {
        inf_err_msg("failed to start command buffer!");
        exit(1);
    }

    inf_debug_msg("started command buffer!");
}

static void _vk_endCommandBuffer(VkCommandBuffer buffer) {
    if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
        inf_err_msg("failed to end command buffer!");
        exit(1);
    }

    inf_debug_msg("ended command buffer!");
}


static void _vk_submitCommandBuffer(VkCommandBuffer* buffer) {
    inf_debug_msg("submitting command buffer...");

    VkSemaphore waitsemaphores[] = { s.imgavailablesem };
    VkPipelineStageFlags waitstages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSemaphore signalsemaphores[] = { s.renderfinsem };

    VkSubmitInfo submitinfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = NULL,

            .waitSemaphoreCount = sizeof(waitsemaphores)/sizeof(waitsemaphores[0]),
            .pWaitSemaphores = waitsemaphores,
            .pWaitDstStageMask = waitstages,

            .commandBufferCount = 1,
            .pCommandBuffers = buffer,

            .signalSemaphoreCount = sizeof(signalsemaphores)/sizeof(signalsemaphores[0]),
            .pSignalSemaphores = signalsemaphores,
        };

    if (vkQueueSubmit(s.graphicsqueue, 1, &submitinfo, s.inflightfence) != VK_SUCCESS) {
        inf_err_msg("failed to submit command buffer!");
        exit(1);
    }

    inf_debug_msg("submitted command buffer!");
}
