#define CL_TARGET_OPENCL_VERSION 220

#include <CL/cl.h>

#include "infsys.h"

#include "ui/ui.h"
#include "cpu/info.h"

static void cl_gpu_get_name(char *name_out, size_t size)
{
    cl_platform_id platform_id = NULL;
    cl_device_id device = NULL;   

    cl_int ret = clGetPlatformIDs(1, &platform_id, NULL);
    
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, 
            &device, NULL);

    if (ret == CL_SUCCESS)
    {
        clGetDeviceInfo(device, CL_DEVICE_NAME, size,
                        name_out, NULL);
    }
}

static int infsys_run(void)
{
    struct ui_widget_next nxt = {.is_center = 1};
    struct ui_window *window;
    struct cpu_info cpu;

    cpu_identify(&cpu);

    window = ui_new_window(WINDOW_NAME, WINDOW_WIDTH,
                           WINDOW_HEIGHT);

    ui_label(window, "Name", 45, 22);
    ui_textbox_n(window, cpu.name, 80, 20, 310, 17, nxt);

    char cores[3] = {'\0'}, threads[3] = {'\0'};

    snprintf(threads, 3, "%d", (int) cpu.threads);
    snprintf(cores, 3, "%d", (int) cpu.cores);

    ui_textbox_n(window, cores, 395, 20, 25, 17, nxt);
    ui_textbox_n(window, threads, 425, 20, 25, 17, nxt);

    char family[3]   = {'\0'}, model[3] = {'\0'},
         stepping[3] = {'\0'};
    
    snprintf(family, 3, "%01X", (int) cpu.family);
    snprintf(stepping, 3, "%01X", (int) cpu.stepping);
    snprintf(model, 3, "%01X", (int) cpu.model);

    ui_label(window, "Family", 44, 42);
    ui_textbox_n(window, family, 80, 40, 50, 17, nxt);

    ui_label(window, "Model", 200, 42);
    ui_textbox_n(window, model, 235, 40, 50, 17, nxt);

    ui_label(window, "Stepping", 350, 42);
    ui_textbox_n(window, stepping, 400, 40, 50, 17, nxt);

    char features[1024];
    cpu_features_str(&cpu, features, 1024);

    ui_label(window, "Features", 32, 62);
    ui_textbox(window, features, 80, 60, 370, 80);

    char caches[4][128];

    for (int i = 0; i < 4; i++)
    {
        struct cpu_cache cache = cpu.caches[i];
        snprintf(caches[i], 128, "%d KB (%d-way)",
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

    char gpu_name[64];
    cl_gpu_get_name(gpu_name, 64);

    ui_label(window, "GPU", 50, 221);
    ui_textbox_n(window, gpu_name, 80, 220, 370, 17, nxt);

    ui_window_open(window);

    return UI_INIT_SUCCESS;
}

INFSYS_MAIN_RUN(ui_init(infsys_run))