#include "wl.h"

#include <stdio.h>
#include <stdlib.h>

#include <wayland-client-core.h>
#include <wayland-client.h>

static struct state {
    struct wl_display* display;
} s;

static void wl_init(void) {
    s.display = wl_display_connect(NULL);
    if (!s.display) {
        inf_err_msg("failed to connect to wayland display!");
        exit(1);
    }
    inf_debug_msg("connected wayland display!");

    inf_debug_msg("initialized wayland!");
}
static void wl_exit(void) {
    inf_debug_msg("disconnected wayland display!");

    inf_debug_msg("exited wayland!");
}

static bool wl_poll(inf_window* window) {
    inf_debug_msg("dispatching display!");

    return wl_display_dispatch(s.display) != -1;
}

const inf_platImpl inf_wl_impl = (inf_platImpl){
        .init = wl_init,
        .exit = wl_exit,

        .poll = wl_poll,
    };
