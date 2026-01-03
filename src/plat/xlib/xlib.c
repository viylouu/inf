#include "xlib.h"

#ifdef INF_DEBUG
#include <string.h>
#endif

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <time.h>

typedef struct xlib_windowData {
    Window window;
    Atom delwindow;
} xlib_windowData;


static struct state {
    Display* maindisp;
    Window rootwin;
} s;

static void xlib_init(void) {
    inf_debug_msg("initializing xlib...");

    s.maindisp = XOpenDisplay(0);
    inf_debug_msg("opened x display at 0!");

    s.rootwin = XDefaultRootWindow(s.maindisp);
    inf_debug_msg("made default root window!");

    inf_debug_msg("initialized xlib!");
}
static void xlib_exit(void) {
    inf_debug_msg("exiting xlib...");

    XFlush(s.maindisp);
    XCloseDisplay(s.maindisp);
    inf_debug_msg("closed x display!");

    inf_debug_msg("exited xlib!");
}

static void xlib_destWindow(inf_window* window);
static bool xlib_poll(inf_window* window) {
    inf_debug_msg("polling events...");

    xlib_windowData* w_data = window->data;

    u32 count = 0;
    while (XPending(s.maindisp) > 0) {
        XEvent event = {};
        XNextEvent(s.maindisp, &event);

        switch (event.type) {
            case KeyPress:
            case KeyRelease: {
                inf_debug_msg("key press/release event detected!");
                XKeyEvent* kp_event = (XKeyEvent*)&event;
                // todo: handle
            } break;
            case ClientMessage: {
                inf_debug_msg("client message event detected!");
                XClientMessageEvent* cm_event = (XClientMessageEvent*)&event;
                if ((Atom)cm_event->data.l[0] == w_data->delwindow) { // what the absolute fuck
                    inf_debug_msg("got message \"WM_DELETE_WINDOW\", returning false from poll!");
                    return false;
                }
            } break;
            default:
                inf_warn_fmt("unhandled event type! (%d)", event.type);
                break;
        }

        inf_debug_msg("polled event!");
        count++;
    }

    inf_debug_fmt("polled %d events!", count);

    return true;
}

static inf_window xlib_makeWindow(inf_windowDesc desc) {
    inf_debug_msg("creating window...");

    inf_window window = (inf_window){ 
            .data = inf_malloc(sizeof(xlib_windowData)),
            .desc = desc,
        };
    xlib_windowData* data = window.data;

    int w_x = 0;
    int w_y = 0;
    int w_width  = desc.width;
    int w_height = desc.height;
    int w_border = 0;
    int w_depth = CopyFromParent;
    int w_class = CopyFromParent;
    Visual* w_visual = CopyFromParent;

    int attribValMask = CWBackPixel | CWEventMask;
    XSetWindowAttributes w_attribs = {};
    w_attribs.background_pixel = 0xff000000;
    w_attribs.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ExposureMask;

    data->window = XCreateWindow(s.maindisp, s.rootwin,
            w_x, w_y, w_width, w_height,
            w_border, w_depth, w_class, w_visual,
            attribValMask, &w_attribs
            );

    XMapWindow(s.maindisp, data->window);

    XStoreName(s.maindisp, data->window, desc.title);

    data->delwindow = XInternAtom(s.maindisp, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(s.maindisp, data->window, &data->delwindow, 1)) {
        inf_err_msg("failed to registed WM_DELETE_WINDOW property!");
    }

    if (!desc.resizable) {
        XSizeHints* sizehints = XAllocSizeHints();
        sizehints->flags = PMinSize | PMaxSize;
        sizehints->min_width = sizehints->max_width = desc.width;
        sizehints->min_height = sizehints->max_height = desc.height;
        XSetWMNormalHints(s.maindisp, data->window, sizehints);
        XFree(sizehints);
    }

    inf_debug_msg("created window!");

    return window;
}
static void xlib_destWindow(inf_window* window) {
    inf_debug_msg("deleting window...");
    xlib_windowData* data = window->data;
    XDestroyWindow(s.maindisp, data->window);
    inf_free(data);
    window->data = 0;
    inf_debug_msg("deleted window!");
}

static f64 xlib_getTime(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    f64 time = t.tv_sec + t.tv_nsec / 1e9;
    inf_debug_fmt("got time! returned %f", time);
    return time;
}

static const char** xlib_VK_reqInstExts(u32* count) {
    static const char* arr[] = {
            "VK_KHR_surface",
            "VK_KHR_xlib_surface"
        };
    *count = sizeof(arr)/sizeof(arr[0]);

#ifdef INF_DEBUG
    char buf[256] = "";
    for (int i = 0; i < *count; ++i) {
        strcat(buf, arr[i]);
        if (i != *count - 1) strcat(buf, ", ");
    }
    inf_debug_fmt("queried required vulkan instance extensions! got:\n\t%s", buf);
#endif

    return arr;
}

static void* xlib_XLIB_getDisplay(void) {
    inf_debug_msg("acquiring xlib display!");
    return s.maindisp;
}
static void* xlib_XLIB_getWindow(inf_window* window) {
    inf_debug_msg("acquiring xlib window!");
    xlib_windowData* data = window->data;
    return &data->window;
}

const inf_platImpl inf_xlib_impl = (inf_platImpl){
        .api = "xlib",

        .init = xlib_init,
        .exit = xlib_exit,

        .poll = xlib_poll,

        .makeWindow = xlib_makeWindow,
        .destWindow = xlib_destWindow,

        .getTime = xlib_getTime,

        .VK_reqInstExts = xlib_VK_reqInstExts,

        .XLIB_getDisplay = xlib_XLIB_getDisplay,
        .XLIB_getWindow = xlib_XLIB_getWindow,
    };
