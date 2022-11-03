#if _WIN32

#include "ui/ui.h"

#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#include <windows.h>

#define CLASS_NAME              L"__WinProc__"

#define WIDGET_TEXT_BUF_SIZE    256

#define WINDOW_NAME_BUF_SIZE    128
#define WINDOW_STYLES           WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME \
                                ^ WS_MAXIMIZEBOX

#define INSTANCE                GetModuleHandle(NULL)

#define ascii_to_wide(dest, src, size)                              \
    MultiByteToWideChar(CP_UTF8, 0, src, -1, dest, (int) size)

typedef wchar_t *wstring;

struct ui_window
{
    HWND hwnd;
    
    ui_close_fn close_fn;
};

static size_t win_ui_created_windows = 0;

static void register_class(WNDPROC proc, wstring name)
{
    WNDCLASS wc = {0};

    wc.lpfnWndProc   = proc;
    wc.hInstance     = INSTANCE;
    wc.lpszClassName = name;

    RegisterClass(&wc);
}

static void fix_fonts(HWND window)
{
    WPARAM font = (WPARAM) GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(window, WM_SETFONT, font, 1);
}

static LRESULT CALLBACK window_proc(HWND hwnd, UINT message,
                                    WPARAM w_param, LPARAM l_param)
{
    struct ui_window *window;

    if (message == WM_NCCREATE)
    {
        CREATESTRUCT* create = (CREATESTRUCT*) l_param;
        window = (struct ui_window*) create->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) window);

        window->hwnd = hwnd;
    }
    else
    {
        window = (struct ui_window*) GetWindowLongPtr(hwnd,
                                                      GWLP_USERDATA);
    }

    switch (message)
    {
    case WM_DESTROY:
        if (window->close_fn)
            window->close_fn(window);
        
        if (!(--win_ui_created_windows))
            PostQuitMessage(0);

        return 0;

    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW));
        EndPaint(hwnd, &ps);
        return 0;
    }

    return DefWindowProc(hwnd, message, w_param, l_param);
}

int ui_init(ui_init_fn fn)
{
    MSG msg = {0};

    register_class(window_proc, CLASS_NAME);

    if (fn() < 0) return 1;

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

struct ui_window *ui_new_window(const string name, uint32_t width,
                                uint32_t height)
{
    struct ui_window *tmp = malloc(sizeof(*tmp));
    wchar_t buffer[WINDOW_NAME_BUF_SIZE];

    ascii_to_wide(buffer, name, WINDOW_NAME_BUF_SIZE);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, buffer, WINDOW_STYLES,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, INSTANCE, tmp
    );

    tmp->close_fn = NULL;
    tmp->hwnd = hwnd;

    win_ui_created_windows++;

    return tmp;
}

void ui_window_on_close(struct ui_window *win, ui_close_fn fn)
{
    win->close_fn = fn;
}

void ui_window_widget(struct ui_window *win, struct ui_widget widget)
{
    wchar_t buffer[WIDGET_TEXT_BUF_SIZE];
    HWND hwnd;

    ascii_to_wide(buffer, widget.text, WIDGET_TEXT_BUF_SIZE);

    switch (widget.type)
    {
    case UI_LABEL:
        hwnd = CreateWindow(
            L"static", buffer, SS_EDITCONTROL | WS_CHILD | WS_VISIBLE,
            widget.x, widget.y, widget.width, widget.height, win->hwnd, NULL,
            NULL, NULL
        );
        break;

    case UI_TEXTBOX:
        hwnd = CreateWindow(
            L"edit", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_READONLY
            | ES_MULTILINE, widget.x, widget.y, widget.width, widget.height,
            win->hwnd, NULL, NULL, NULL
        );

        SetWindowTextW(hwnd, buffer);
        break;
    }

    fix_fonts(hwnd);
}

void ui_window_open(struct ui_window *win)
{
    ShowWindow(win->hwnd, 1);
}

#endif /* _WIN32 */
