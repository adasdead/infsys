#warning WINDOWS NOT SUPPORTED

#if 0

#include "ui/ui.h"

#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#include <windows.h>

#define WINDOW_NAME_BUF_SIZE    128
#define WINDOW_MAX_WIDGETS      64

#define CLASS_NAME              L"__WinProc__"

#define WIDGET_NAME_BUF_SIZE    256

#define ascii_to_wide(dest, src, size)          \
    MultiByteToWideChar(CP_UTF8, 0, src, -1,    \
                        dest, (int) size)

#define get_instance() GetModuleHandle(NULL);

typedef wchar_t *wstring;

static size_t created_windows = 0;

struct ui_window
{
    HWND hwnd;

    HWND wigdets[WINDOW_MAX_WIDGETS];

    ui_close_fn close_fn;

    size_t widgets_count;

    uint32_t height;
    uint32_t width;
};

static void register_class(WNDPROC proc, wstring name)
{
    WNDCLASS wc = {0};

    wc.lpfnWndProc   = proc;
    wc.hInstance     = get_instance();
    wc.lpszClassName = name;

    RegisterClass(&wc);
}

static void fix_fonts(HWND *window)
{
    WPARAM font = (WPARAM) GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(window, WM_SETFONT, font, TRUE);
}

static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg,
                                    WPARAM wParam, LPARAM lParam)
{
    struct ui_window *window;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;
        window = (struct ui_window*) pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);

        window->hwnd = hwnd;
    }
    else
    {
        window = (struct ui_window*) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    switch (uMsg)
    {
    case WM_DESTROY:
        if (window->close_fn)
            window->close_fn(window);

        for (int i = 0; i < window->widgets_count; i++)
            GUI_WIDGET_FREE(window->widgets[i]);

        if (!(--__created_windows))
            PostQuitMessage(0);

        return 0;

    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW));
        EndPaint(hwnd, &ps);
        return 0;

    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// GUI

void gui_init(int (*init_fn)())
{
    register_class(window_proc, CLASS_NAME);

    if (init_fn() < 0) return;

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Window

window_t new_window(cstring_t name, pos_t size)
{
    wchar_t name_buf[WINDOW_NAME_BUF_SIZE];

    w32_ascii_to_wide(name_buf, name, WINDOW_NAME_BUF_SIZE);

    window_t win_ptr = malloc(sizeof(struct _window_t));

    HWND hwnd = CreateWindowEx(
        0, L"_WinProc_", name_buf, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y,
        NULL, NULL, __w32_hinstance, win_ptr
    );

    if (hwnd == NULL)
    {
        free(win_ptr);
        return NULL;
    }

    win_ptr->widget = (widget_t) { W_WINDOW, win_ptr };
    win_ptr->widgets_count = 0;
    win_ptr->close_fn = NULL;
    win_ptr->hwnd = hwnd;
    win_ptr->size = size;

    __created_windows++;

    return win_ptr;
}

void window_attach(window_t win, void *widget_p)
{
    widget_t *widget = (widget_t*) widget_p;

    switch (widget->type)
    {
    case W_LABEL:
        label_t lbl = widget->self;

        lbl->hwnd = CreateWindow(
            L"static", lbl->value, SS_EDITCONTROL | WS_CHILD | WS_VISIBLE,
            lbl->pos.x, lbl->pos.y, lbl->size.x, lbl->size.y,
            win->hwnd, NULL, NULL, NULL
        );

        break;

    case W_TXTBOX:
        txtbox_t box = widget->self;

        box->hwnd = CreateWindow(
            L"edit", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_READONLY
            | ES_MULTILINE, box->pos.x, box->pos.y, box->size.x, box->size.y,
            win->hwnd, NULL, NULL, NULL
        );

        SetWindowTextW(box->hwnd, box->value);
        break;
    }

    win->widgets[win->widgets_count++] = widget;

    __fix_fonts(widget);
}

void window_on_close(window_t win, void (*close_fn)(window_t))
{
    win->close_fn = close_fn;
}

void window_no_resizable(window_t win)
{
    LONG styles = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
    SetWindowLong(win->hwnd, GWL_STYLE, styles);
}

void window_open(window_t win)
{
    ShowWindow(win->hwnd, 1);
}

#endif /* _WIN32 */
