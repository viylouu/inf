// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

//#include <string.h>

static void _vk_createFramebuffers(inf_window* window) {
    inf_debug_msg("creating framebuffers...");

    vk_windowRdata* rd = window->rdata;

    rd->scfbufamt = rd->scimgviewamt;
    rd->swapchainfbufs = inf_malloc(sizeof(VkFramebuffer) * rd->scfbufamt);

    for (u32 i = 0; i < rd->scfbufamt; ++i) {
        VkImageView attachments[] = {
                rd->swapchainimgviews[i],
            };

        VkFramebufferCreateInfo fbinfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = NULL,

                .flags = 0,

                .renderPass = rd->renderpass,

                .attachmentCount = sizeof(attachments)/sizeof(attachments[0]),
                .pAttachments = attachments,

                .width = rd->swapchainext.width,
                .height = rd->swapchainext.height,

                .layers = 1,
            };

        if (vkCreateFramebuffer(s.device, &fbinfo, NULL, &rd->swapchainfbufs[i]) != VK_SUCCESS) {
            inf_err_msg("failed to create framebuffer!");
            exit(1);
        }
    }

    inf_debug_msg("created framebuffers!");
}

static void _vk_deleteFramebuffers(inf_window* window) {
    inf_debug_msg("deleting framebuffers...");

    vk_windowRdata* rd = window->rdata;

    for (u32 i = 0; i < rd->scfbufamt; ++i)
        vkDestroyFramebuffer(s.device, rd->swapchainfbufs[i], NULL);

    inf_free(rd->swapchainfbufs);

    inf_debug_msg("deleted framebuffers!");
}
