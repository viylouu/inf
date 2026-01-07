#include "../main.h"

#include <stdio.h>

#include "../src/plat/xlib/xlib.h"
#include "../src/rend/vk/vk.h"

int main() {
    inf_plat_useImpl(&inf_xlib_impl);
    inf_rend_useImpl(&inf_vk_impl);

    inf_plat_init();
    inf_rend_init();

    inf_window win = inf_plat_makeWindow((inf_windowDesc){
                .width = 800,
                .height = 600,
                .title = "triangle",

                .resizable = false,
                .hidden = false,
            });

    u64 frames = 0;
    f64 start = inf_plat_getTime();
    while (inf_plat_poll(&win)) {
        f64 time = inf_plat_getTime();
        if (time - start >= 1) {
            printf("FPS: %lu\n", frames);
            start = time;
            frames = 0;
        }
        frames++;

        inf_rend_frameStart(&win, (f32[4]){.2f,.4f,.3f, 1.f});
        inf_rend_frameEnd(&win);
    }

    inf_plat_destWindow(&win);

    inf_rend_exit();
    inf_plat_exit();

    return 0;
}
