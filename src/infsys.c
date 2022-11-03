#include "infsys.h"

#include "ui/ui.h"
#include "cpu/info.h"

static void on_closed(struct ui_window *win)
{
    puts("CLOSED");
}

static int run(void)
{
    struct ui_window *window;
    struct cpu_info info;

    cpu_identify(&info);

    window = ui_new_window(WINDOW_NAME, WINDOW_WIDTH,
                           WINDOW_HEIGHT);

    ui_label(window, "Name", 65, 22);
    ui_textbox(window, info.name, 100, 20, 310, 17);

    char cores[3] = {'\0'}, threads[3] = {'\0'};

    sprintf(threads, "%d", (int) info.threads);
    sprintf(cores, "%d", (int) info.cores);

    ui_textbox(window, cores, 415, 20, 25, 17);
    ui_textbox(window, threads, 445, 20, 25, 17);

    char features[512];
    cpu_features_str(&info, features, 512);

    ui_label(window, "Features", 54, 40);
    ui_textbox(window, features, 100, 40, 370, 80);

    ui_window_on_close(window, on_closed);
    ui_window_open(window);

    return UI_INIT_SUCCESS;
}

int main(void)
{
    return ui_init(run);
}
