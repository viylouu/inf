// DO NOT COMPILE SEPERATELY
// INCLUDED IN vk.c

#pragma once

//#include "vk.h"
#include "loc.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

static void _vk_createSurface(inf_window* window) {
    inf_debug_msg("creating surface...");

    vk_windowRdata* rd = window->rdata;

    if (!strcmp(inf_cur_plat_impl->api, "xlib")) {
        VkXlibSurfaceCreateInfoKHR createinfo = {
                .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
                .pNext = NULL,

                .flags = 0,

                .dpy = (Display*)inf_plat_XLIB_getDisplay(),
                .window = *(Window*)inf_plat_XLIB_getWindow(window),
            };

        if (vkCreateXlibSurfaceKHR(s.instance, &createinfo, NULL, &rd->surface) != VK_SUCCESS) {
            inf_err_msg("failed to create window surface! (xlib)");
            exit(1);
        }
    }

    inf_debug_msg("created surface!");
}

static void _vk_deleteSurface(inf_window* window) {
    vk_windowRdata* rd = window->rdata;
    vkDestroySurfaceKHR(s.instance, rd->surface, NULL);
    inf_debug_msg("deleted surface!");
}
