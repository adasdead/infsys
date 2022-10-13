#include "infsys.h"

#include "windows/winapi.h"

#include "cpu/cpuinfo.h"
#include "cpu/cpucache.h"

#include "definitions.h"

#include "gui.h"

static int application_init(void)
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

#if 0
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

    for (int i = 0; i < 4; i++)
    {
        printf("%d %d\n",
            cpuinfo.caches[i].size / SIZE_KB,
            cpuinfo.caches[i].ways
        );
    }

    gui_init(application_init);

    return EXIT_SUCCESS;
}