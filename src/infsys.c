#include "infsys.h"

#include "cpu/cpuinfo.h"
#include "cpu/cpucache.h"

#include "gui.h"

#define WINDOW_NAME                  "InfSyS v1.0"
#define WINDOW_WIDTH                 500
#define WINDOW_HEIGHT                500

int on_program_init(void);

#if 0

#include "windows/winapi.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    __w32_hinstance = hInstance;
#else
int main(void)
{
#endif
    cpuinfo_identify();

    gui_init(on_program_init);
    
    return EXIT_SUCCESS;
}

int on_program_init(void)
{
    window_t win = new_window(WINDOW_NAME, (pos_t) {
        WINDOW_WIDTH, WINDOW_HEIGHT
    });

    LABEL(win, 65, 22, "Name");
    TXTBOX(win, 100, 20, 310, 17, cpuinfo.name);

    char cores[3] = {'\0'}, threads[3] = {'\0'};

    sprintf(threads, "%d", (int) cpuinfo.threads);
    sprintf(cores, "%d", (int) cpuinfo.cores);

    TXTBOX(win, 415, 20, 25, 17, cores);
    TXTBOX(win, 445, 20, 25, 17, threads);

    char features[512];
    cpuinfo_features_str(features, 512);

    LABEL(win, 52, 40, "Features");
    TXTBOX(win, 100, 40, 370, 80, features);
    
    window_no_resizable(win);
    window_open(win);

    return GUI_INIT_SUCCESS;
}