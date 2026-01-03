// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

static bool _vk_validationSupport(void) {
    inf_debug_msg("checking validation layer support...");

    u32 layercount;
    vkEnumerateInstanceLayerProperties(&layercount, NULL);

    inf_debug_fmt("found %d available layers!", layercount);

    VkLayerProperties* available = inf_malloc(sizeof(VkLayerProperties) * layercount);
    vkEnumerateInstanceLayerProperties(&layercount, available);

    for (u32 i = 0; i < sizeof(validations)/sizeof(validations[0]); ++i) {
        for (u32 j = 0; j < layercount; ++j) if (strcmp(validations[i], available[j].layerName) == 0)
            goto next;
        inf_err_fmt("unsupported layer %s!", validations[i]);
        return false;
next:;
    }
    
    inf_debug_msg("validation layers supported!");
    return true;
}

static void _vk_createInstance(void) {
    inf_debug_msg("creating instance...");

    if (validation && !_vk_validationSupport()) {
        inf_err_msg("validation layers not available!");
        exit(1);
    }

    VkApplicationInfo appinfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = NULL,

            .pApplicationName = "appname",
            .applicationVersion = VK_MAKE_VERSION(1,0,0),

            .pEngineName = "inf",
            .engineVersion = VK_MAKE_VERSION(INF_VER_MAJOR, INF_VER_MINOR, INF_VER_PATCH),

            .apiVersion = VK_API_VERSION_1_0,
        };

    u32 extamt;
    const char** exts = inf_plat_VK_reqInstExts(&extamt);

    inf_debug_msg("checking if acquired exts are available...");

    u32 actexts;
    vkEnumerateInstanceExtensionProperties(NULL, &actexts, NULL);
    
    VkExtensionProperties* props = inf_malloc(sizeof(VkExtensionProperties) * actexts);
    vkEnumerateInstanceExtensionProperties(NULL, &actexts, props);

    inf_debug_fmt("found %d available exts!", actexts);

    for (int i = 0; i < extamt; ++i) {
        for (u32 j = 0; j < actexts; ++j) if (strcmp(exts[i], props[j].extensionName) == 0)
            goto next;
        inf_err_fmt("unsupported extension %s!", exts[i]);
        exit(1);
next:;
    }

    inf_free(props);
    inf_debug_msg("all acquired exts available!");

    VkInstanceCreateInfo createinfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .pApplicationInfo = &appinfo,

            .enabledLayerCount = 0,
            .ppEnabledLayerNames = NULL,

            .enabledExtensionCount = extamt,
            .ppEnabledExtensionNames = exts,
        };

    if (validation) {
        createinfo.enabledLayerCount = sizeof(validations)/sizeof(validations[0]);
        createinfo.ppEnabledLayerNames = validations;
    }

    if (vkCreateInstance(&createinfo, NULL, &s.instance) != VK_SUCCESS) {
        inf_err_msg("failed to create vulkan instance!");
        exit(1);
    }

    inf_debug_msg("created instance!");
}

static void _vk_deleteInstance(void) {
    vkDestroyInstance(s.instance, NULL);
    inf_debug_msg("deleted instance!");
}
