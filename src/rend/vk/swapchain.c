// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include "device.c"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

VkSurfaceFormatKHR _vk_chooseSwapSurfaceFormat(VkSurfaceFormatKHR* availablefmts, u32 fmtamt) {
    for (u32 i = 0; i < fmtamt; ++i) {
        VkSurfaceFormatKHR aft = availablefmts[i];
        if (aft.format == VK_FORMAT_B8G8R8A8_SRGB && aft.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return aft;
    }

    return availablefmts[0];
}

VkPresentModeKHR _vk_chooseSwapPresentMode(VkPresentModeKHR* availablepmds, u32 pmdamt) {
    for (u32 i = 0; i < pmdamt; ++i) {
        VkPresentModeKHR apmd = availablepmds[i];
        if (apmd == VK_PRESENT_MODE_MAILBOX_KHR)
            return apmd;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

// !!!! using window!
VkExtent2D _vk_chooseSwapExtent(inf_window* window, VkSurfaceCapabilitiesKHR caps) {
    if (caps.currentExtent.width == UINT32_MAX) {
        VkExtent2D actextent = {
                window->desc.width,
                window->desc.height,
            };

        if (actextent.width < caps.minImageExtent.width)
            actextent.width = caps.minImageExtent.width;
        if (actextent.height < caps.minImageExtent.height)
            actextent.height = caps.minImageExtent.height;
        if (actextent.width > caps.maxImageExtent.width)
            actextent.width = caps.maxImageExtent.width;
        if (actextent.height > caps.maxImageExtent.height)
            actextent.height = caps.maxImageExtent.height;

        return actextent;
    }

    return caps.currentExtent;
}

static void _vk_createSwapchain(inf_window* window) {
    inf_debug_msg("creating swapchain...");

    _schainSupportDetails scsup = _vk_querySwapchainSupport(s.physicaldevice);

    VkSurfaceFormatKHR surffmt = _vk_chooseSwapSurfaceFormat(scsup.fmts, scsup.fmtamt);
    VkPresentModeKHR pmd = _vk_chooseSwapPresentMode(scsup.presentmodes, scsup.presentmodeamt);
    VkExtent2D ext = _vk_chooseSwapExtent(window, scsup.caps);

    u32 imgs = scsup.caps.minImageCount + 1;
    if (scsup.caps.maxImageCount > 0 && imgs > scsup.caps.maxImageCount)
        imgs = scsup.caps.maxImageCount;
    
    _queueFamine famine = _vk_findQueueFamilies(s.physicaldevice);
    u32 inds[] = { famine.graphicsfam, famine.presentfam };
    bool isameboth = inds[0] == inds[1];

    VkSwapchainCreateInfoKHR createinfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = NULL,

            .flags = 0,
            
            .surface = s.surface,

            .minImageCount = imgs,
            .imageFormat = surffmt.format,
            .imageColorSpace = surffmt.colorSpace,
            .imageExtent = ext,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

            .imageSharingMode = isameboth? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
            .queueFamilyIndexCount = isameboth? 0 : 2,
            .pQueueFamilyIndices = isameboth? NULL : inds,

            .preTransform = scsup.caps.currentTransform,

            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            
            .presentMode = pmd,
            .clipped = true,

            .oldSwapchain = NULL,
        };

    if (vkCreateSwapchainKHR(s.device, &createinfo, NULL, &s.swapchain) != VK_SUCCESS) {
        inf_err_msg("failed to create swapchain!");
        exit(1);
    }

    vkGetSwapchainImagesKHR(s.device, s.swapchain, &s.scimgamt, NULL);
    s.swapchainimgs = inf_malloc(sizeof(VkImage) * s.scimgamt);
    vkGetSwapchainImagesKHR(s.device, s.swapchain, &s.scimgamt, s.swapchainimgs);

    s.swapchainimgfmt = surffmt.format;
    s.swapchainext = ext;

    inf_debug_msg("created swapchain!");
}

static void _vk_deleteSwapchain(void) {
    inf_free(s.swapchainimgs);
    vkDestroySwapchainKHR(s.device, s.swapchain, NULL);

    inf_debug_msg("deleted swapchain!");
}
