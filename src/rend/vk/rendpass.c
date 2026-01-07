// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

//#include <string.h>

static void _vk_createRenderPass(inf_window* window) {
    inf_debug_msg("creating render pass...");

    vk_windowRdata* rd = window->rdata;

    VkAttachmentDescription colorattach = {
            .format = rd->swapchainimgfmt,
            .samples = VK_SAMPLE_COUNT_1_BIT,

            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

    VkAttachmentReference colorattachref = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };

    VkSubpassDescription subpass = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,

            .colorAttachmentCount = 1,
            .pColorAttachments = &colorattachref,
        };

    VkSubpassDependency dependency = {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,

            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,

            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
        };

    VkRenderPassCreateInfo renderpassinfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .attachmentCount = 1,
            .pAttachments = &colorattach,

            .subpassCount = 1,
            .pSubpasses = &subpass,

            .dependencyCount = 1,
            .pDependencies = &dependency,
        };

    if (vkCreateRenderPass(s.device, &renderpassinfo, NULL, &rd->renderpass) != VK_SUCCESS) {
        inf_err_msg("failed to create render pass!");
        exit(1);
    }

    inf_debug_msg("created render pass!");
}

static void _vk_deleteRenderPass(inf_window* window) {
    inf_debug_msg("deleting render pass...");

    vk_windowRdata* rd = window->rdata;
    vkDestroyRenderPass(s.device, rd->renderpass, NULL);

    inf_debug_msg("deleted render pass!");
}


static void _vk_startRenderPass(inf_window* window, u32 index, f32 clear[4], u32 frame) {
    vk_windowRdata* rd = window->rdata;

    VkClearValue clearcolor = {{{clear[0],clear[1],clear[2],clear[3]}}};

    VkRenderPassBeginInfo renderpassinfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = NULL,

            .renderPass = rd->renderpass,
            .framebuffer = rd->swapchainfbufs[index],

            .renderArea.offset = {0,0},
            .renderArea.extent = rd->swapchainext,

            .clearValueCount = 1,
            .pClearValues = &clearcolor,
        };

    vkCmdBeginRenderPass(s.cmdbuffers[frame], &renderpassinfo, VK_SUBPASS_CONTENTS_INLINE);
}

static void _vk_endRenderPass(u32 frame) {
    vkCmdEndRenderPass(s.cmdbuffers[frame]);
}
