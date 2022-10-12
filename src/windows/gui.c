#include "gui.h"

#include "windows/winapi.h"

#define WINDOW_NAME_BUF_SIZE            128
#define WINDOW_MAX_WIDGETS              64

#define LABEL_BUF_SIZE                  256
#define TXTBOX_BUF_SIZE                 256

static size_t                           __created_windows = 0;

struct _window_t
{
    widget_t widget, *widgets[WINDOW_MAX_WIDGETS];
    void (*close_fn)(window_t);
    size_t widgets_count;
    pos_t size;
    HWND hwnd;
};

struct _label_t
{
    widget_t widget;
    window_t parent;
    wchar_t value[LABEL_BUF_SIZE];
    pos_t size, pos;
    HWND hwnd;
};

struct _txtbox_t
{
    widget_t widget;
    window_t parent;
    wchar_t value[TXTBOX_BUF_SIZE];
    pos_t size, pos;
    HWND hwnd;
};

static HWND __widget_to_HWND(widget_t *widget)
{
    switch (widget->type)
    {
    case W_WINDOW:
        return ((window_t) widget->self)->hwnd;
    case W_LABEL:
        return ((label_t) widget->self)->hwnd;
    case W_TXTBOX:
        return ((txtbox_t) widget->self)->hwnd;
    }

    return NULL;
}

static void __fix_fonts(widget_t *widget)
{
    WPARAM font = (WPARAM) GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(__widget_to_HWND(widget), WM_SETFONT, font, TRUE);
}

static LRESULT CALLBACK __window_proc(HWND hwnd, UINT uMsg,
                                      WPARAM wParam, LPARAM lParam)
{
    window_t window;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;
        window = (window_t) pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);

        window->hwnd = hwnd;
    }
    else
    {
        window = (window_t) GetWindowLongPtr(hwnd, GWLP_USERDATA);
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
    w32_register_class(__window_proc, "_WinProc_");

    if (init_fn() < 0) return;

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

widget_t* gui_to_widget(void *widget)
{
    struct
    {
        widget_t widget;

    } *data = widget;

    return &(data->widget);    
}

void __gui_free(widget_t *widget)
{
    if (!widget) return;

    DestroyWindow(__widget_to_HWND(widget));
    free(widget->self);
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

// Label

label_t new_label(cstring_t text, pos_t pos, pos_t size)
{
    label_t lbl_ptr = malloc(sizeof(struct _label_t));
    
    w32_ascii_to_wide(lbl_ptr->value, text, LABEL_BUF_SIZE);
    
    lbl_ptr->widget = (widget_t) { W_LABEL, lbl_ptr };
    lbl_ptr->size = size;
    lbl_ptr->pos = pos;

    return lbl_ptr;
}

// Text Box

txtbox_t new_textbox(cstring_t text, pos_t pos, pos_t size)
{
    txtbox_t box_ptr = malloc(sizeof(struct _txtbox_t));
    
    w32_ascii_to_wide(box_ptr->value, text, LABEL_BUF_SIZE);
    
    box_ptr->widget = (widget_t) { W_TXTBOX, box_ptr };
    box_ptr->size = size;
    box_ptr->pos = pos;

    return box_ptr;
}