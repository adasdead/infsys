#include "infsys.h"

#include "ui/ui.h"

#include "hardware/cpu/info.h"
#include "hardware/gpu.h"

static void add_cpu_section(struct ui_window *window)
{
    struct cpu_info cpu;

    cpu_identify(&cpu);

    ui_label(window, "Name", 15, 22);
    ui_textbox(window, cpu.name, 80, 20, 310, 17, ui_center);

    char cores[3] = {'\0'}, threads[3] = {'\0'};

    snprintf(threads, 3, "%d", (int) cpu.threads);
    snprintf(cores, 3, "%d", (int) cpu.cores);

    ui_textbox(window, cores, 395, 20, 25, 17, ui_center);
    ui_textbox(window, threads, 425, 20, 25, 17, ui_center);

    char family[3]   = {'\0'}, model[3] = {'\0'},
         stepping[3] = {'\0'};
    
    snprintf(family, 3, "%01X", (int) cpu.family);
    snprintf(stepping, 3, "%01X", (int) cpu.stepping);
    snprintf(model, 3, "%01X", (int) cpu.model);

    ui_label(window, "Family", 15, 42);
    ui_textbox(window, family, 80, 40, 50, 17, ui_center);

    ui_label(window, "Model", 170, 42);
    ui_textbox(window, model, 235, 40, 50, 17, ui_center);

    ui_label(window, "Stepping", 335, 42);
    ui_textbox(window, stepping, 400, 40, 50, 17, ui_center);

    char features[1024];
    cpu_features_str(&cpu, features, 1024);

    ui_label(window, "Features", 15, 62);
    ui_textbox(window, features, 80, 60, 370, 80, NULL);

    char caches[4][128];

    for (int i = 0; i < 4; i++)
    {
        struct cpu_cache cache = cpu.caches[i];
        snprintf(caches[i], 128, "%d KB (%d-way)",
                cache.size / SIZE_KB, cache.ways);
    }

    ui_label(window, "L1D Cache", 15, 145);
    ui_textbox(window, caches[CACHE_L1D], 80, 143, 140, 17, NULL);

    ui_label(window, "L1I Cache", 245, 145);
    ui_textbox(window, caches[CACHE_L1I], 310, 143, 140, 17, NULL);

    ui_label(window, "L2 Cache", 15, 165);
    ui_textbox(window, caches[CACHE_L2], 80, 163, 140, 17, NULL);

    ui_label(window, "L3 Cache", 245, 165);
    ui_textbox(window, caches[CACHE_L3], 310, 163, 140, 17, NULL);
}

static void add_other_section(struct ui_window *window)
{
    struct gpu_info gpu_info;
    
    gpu_identify(&gpu_info);

    ui_label(window, "GPU", 15, 221);
    ui_textbox(window, gpu_info.name, 80, 220, 370, 17, ui_center);

    ui_label(window, "GPU Vendor", 15, 241);
    ui_textbox(window, gpu_info.vendor, 80, 240, 370, 17, ui_center);
}

static int infsys_run(void)
{
    struct ui_window *window;

    window = ui_new_window(WINDOW_NAME, WINDOW_WIDTH,
                           WINDOW_HEIGHT);

    add_cpu_section(window);
    ui_separator(window, 0, 200, 500);
    add_other_section(window);
    ui_window_open(window);

    return UI_INIT_SUCCESS;
}

INFSYS_MAIN_RUN(ui_init(infsys_run))