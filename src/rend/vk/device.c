// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

// no clue why its famine but ok
typedef struct _queueFamine {
    u32 graphicsfam;
    bool hasgraphicsfam;

    u32 presentfam;
    bool haspresentfam;

    bool complete;
} _queueFamine;

static _queueFamine _vk_findQueueFamilies(VkPhysicalDevice device) {
    _queueFamine famine = {
            .graphicsfam = 0,
            .hasgraphicsfam = false,

            .presentfam = 0,
            .haspresentfam = false,

            .complete = false,
        };

    u32 queuefamamt = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamamt, NULL);

    VkQueueFamilyProperties* queuefams = inf_malloc(sizeof(VkQueueFamilyProperties) * queuefamamt);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamamt, queuefams);

    for (u32 i = 0; i < queuefamamt; ++i) {
        if (queuefams[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            famine.graphicsfam = i;
            famine.hasgraphicsfam = true;
        }

        VkBool32 canpresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, s.twr->surface, &canpresent); 
        if (canpresent) {
            famine.presentfam = i;
            famine.haspresentfam = true;
        }
    }

    famine.complete = famine.hasgraphicsfam && famine.haspresentfam;

    inf_debug_msg("found queue families! (hi this will probably appear alot... owo :3)");
    if (!famine.complete) {
        inf_warn_msg("queue families incomplete!");
    }

    return famine;
}

static bool _vk_checkDeviceExtensionSupport(VkPhysicalDevice device) {
#ifdef INF_DEBUG
    VkPhysicalDeviceProperties D_props;
    vkGetPhysicalDeviceProperties(device, &D_props);

    inf_debug_fmt("checking device extension support... (on %s)", D_props.deviceName);
#endif

    u32 extamt;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extamt, NULL);

    VkExtensionProperties* props = inf_malloc(sizeof(VkExtensionProperties) * extamt);
    vkEnumerateDeviceExtensionProperties(device, NULL, &extamt, props);

    inf_debug_fmt("found %d available device exts!", extamt);

    for(u32 i = 0; i < devextamt; ++i) {
        for(u32 j = 0; j < extamt; ++j) if (strcmp(devexts[i], props[j].extensionName) == 0)
            goto next;
        inf_warn_fmt("unsupported device extension %s!", devexts[i]);
        inf_free(props);
        return false;
next:;
    }

    inf_free(props);

    inf_debug_msg("device extensions supported!");

    return true;
}

typedef struct _schainSupportDetails {
    VkSurfaceCapabilitiesKHR caps;
    VkSurfaceFormatKHR* fmts;
        u32 fmtamt;
    VkPresentModeKHR* presentmodes;
        u32 presentmodeamt;
} _schainSupportDetails;

// REMEMBER TO FREE fmts AND presentmodes
static _schainSupportDetails _vk_querySwapchainSupport(inf_window* window, VkPhysicalDevice device) {
    _schainSupportDetails details = {
            .caps = 0,

            .fmts = NULL,
            .fmtamt = 0,

            .presentmodes = NULL,
            .presentmodeamt = 0,
        };

    vk_windowRdata* rd = window->rdata;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, rd->surface, &details.caps);

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, rd->surface, &details.fmtamt, NULL);
    details.fmts = inf_malloc(sizeof(VkSurfaceFormatKHR) * details.fmtamt);
    if (details.fmtamt != 0)
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, rd->surface, &details.fmtamt, details.fmts);

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, rd->surface, &details.presentmodeamt, NULL);
    details.presentmodes = inf_malloc(sizeof(VkPresentModeKHR) * details.presentmodeamt);
    if (details.presentmodeamt != 0)
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, rd->surface, &details.presentmodeamt, details.presentmodes);

    inf_debug_msg("queried swapchain support!");

    return details;
}

static bool _vk_isDeviceSuitable(VkPhysicalDevice device) {
    _queueFamine famine = _vk_findQueueFamilies(device);

    bool extssupported = _vk_checkDeviceExtensionSupport(device);

    bool adequateswapchain = false;
    if (extssupported) {
        _schainSupportDetails details = _vk_querySwapchainSupport(&s.tempwindow, device);
        adequateswapchain = details.fmtamt != 0 && details.presentmodeamt != 0;
        inf_free(details.fmts);
        inf_free(details.presentmodes);
    }

    return famine.complete && extssupported && adequateswapchain;
}

static void _vk_pickPhysicalDevice(void) {
    inf_debug_msg("picking physical device...");

    s.physicaldevice = NULL;

    u32 deviceamt;
    vkEnumeratePhysicalDevices(s.instance, &deviceamt, NULL);

    if (deviceamt == 0) {
        inf_err_msg("failed to find any vulkan supporting gpus!");
        exit(1);
    }

    VkPhysicalDevice* devices = inf_malloc(sizeof(VkPhysicalDevice) * deviceamt);
    vkEnumeratePhysicalDevices(s.instance, &deviceamt, devices);

    inf_debug_fmt("found %d devices!", deviceamt);

    for (u32 i = 0; i < deviceamt; ++i) {
        VkPhysicalDevice device = devices[i];
        if (_vk_isDeviceSuitable(device)) {
            s.physicaldevice = device;
            break;
        }
    }

    if (s.physicaldevice == NULL) {
        inf_err_msg("failed to find suitable gpu!");
        exit(1);
    }

#ifdef INF_DEBUG
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(s.physicaldevice, &props);

    inf_debug_fmt("picked physical device! (%s)", props.deviceName);
#endif
}


static void _vk_createLogicalDevice(void) {
    inf_debug_msg("creating logical device...");

    _queueFamine famine = _vk_findQueueFamilies(s.physicaldevice);

    f32 queuepri = 1;

    VkDeviceQueueCreateInfo queuecreateinfos[2];
    // too lazy for anything more clever rn
    u32 uniques;
    u32 fams[2] = { famine.graphicsfam, famine.presentfam };
    if (famine.graphicsfam == famine.presentfam)
        uniques = 1;
    else
        uniques = 2;

    for (u32 i = 0; i < uniques; ++i) {
        queuecreateinfos[i] = (VkDeviceQueueCreateInfo){
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = NULL,

                .flags = 0,

                .queueFamilyIndex = fams[i],
                .queueCount = 1,

                .pQueuePriorities = &queuepri,
            };
    }

    VkPhysicalDeviceFeatures devfeatures = {}; // fuck it i aint setting allat to false

    VkDeviceCreateInfo createinfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .queueCreateInfoCount = uniques,
            .pQueueCreateInfos = queuecreateinfos,

            // deprecated, but idfc imma set it anyways (to 0 ofcourse :3)
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = NULL,

            .enabledExtensionCount = devextamt,
            .ppEnabledExtensionNames = devexts,

            .pEnabledFeatures = &devfeatures,
        };
    
    if (validation) {
        createinfo.enabledLayerCount = validamt;
        createinfo.ppEnabledLayerNames = validations;
    }

    if (vkCreateDevice(s.physicaldevice, &createinfo, NULL, &s.device) != VK_SUCCESS) {
        inf_err_msg("failed to create logical device!");
        exit(1);
    }

    vkGetDeviceQueue(s.device, famine.graphicsfam, 0, &s.graphicsqueue);
    vkGetDeviceQueue(s.device, famine.presentfam, 0, &s.presentqueue);

    inf_debug_msg("created logical device!");
}

static void _vk_deleteDevice(void) {
    vkDestroyDevice(s.device, NULL);
    inf_debug_msg("deleted device!");
}
