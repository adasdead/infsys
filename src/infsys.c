#include "infsys.h"

#include "ui/ui.h"
#include "cpu/info.h"

static int infsys_run(void)
{
    struct ui_window *window;
    struct cpu_info cpu;

    cpu_identify(&cpu);

    window = ui_new_window(WINDOW_NAME, WINDOW_WIDTH,
                           WINDOW_HEIGHT);

    ui_label(window, "Name", 45, 22);
    ui_textbox(window, cpu.name, 80, 20, 310, 17);

    char family[3]   = {'\0'}, model[3] = {'\0'},
         stepping[3] = {'\0'};
    
    sprintf(family, "%01X", (int) cpu.family);
    sprintf(stepping, "%01X", (int) cpu.stepping);
    sprintf(model, "%01X", (int) cpu.model);

    ui_label(window, "Family", 44, 42);
    ui_textbox(window, family, 80, 40, 50, 17);

    ui_label(window, "Model", 200, 42);
    ui_textbox(window, model, 235, 40, 50, 17);

    ui_label(window, "Stepping", 350, 42);
    ui_textbox(window, stepping, 400, 40, 50, 17);

    char cores[3] = {'\0'}, threads[3] = {'\0'};

    sprintf(threads, "%d", (int) cpu.threads);
    sprintf(cores, "%d", (int) cpu.cores);

    ui_textbox(window, cores, 395, 20, 25, 17);
    ui_textbox(window, threads, 425, 20, 25, 17);

    char features[1024];
    cpu_features_str(&cpu, features, 1024);

    ui_label(window, "Features", 32, 62);
    ui_textbox(window, features, 80, 60, 370, 80);

    char caches[4][128];

    for (int i = 0; i < 4; i++)
    {
        struct cpu_cache cache = cpu.caches[i];
        sprintf(caches[i], "%d KB (%d-ways)",
                cache.size / SIZE_KB, cache.ways);
    }

    ui_label(window, "L1D Cache", 20, 145);
    ui_textbox(window, caches[CACHE_L1D], 80, 143, 140, 17);

    ui_label(window, "L1I Cache", 255, 145);
    ui_textbox(window, caches[CACHE_L1I], 310, 143, 140, 17);

    ui_label(window, "L2 Cache", 28, 165);
    ui_textbox(window, caches[CACHE_L2], 80, 163, 140, 17);

    ui_label(window, "L3 Cache", 258, 165);
    ui_textbox(window, caches[CACHE_L3], 310, 163, 140, 17);

    ui_separator(window, 0, 200, 500);

    ui_window_open(window);

    return UI_INIT_SUCCESS;
}

int main(void)
{
    return ui_init(infsys_run);
}
