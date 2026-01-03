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
                .title = "idgaf",

                .resizable = false,
                .hidden = false,
            });

    /*
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
    }
    */

    inf_err_msg("hi it worked! this is the middle :3");

    inf_plat_destWindow(&win);

    inf_rend_exit();
    inf_plat_exit();

    return 0;
}
