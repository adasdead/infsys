#ifndef UI_UI_H
#define UI_UI_H

#include "infsys.h"

#define UI_INIT_SUCCESS 0
#define UI_INIT_FAILED  -1

#define ui_label(win, txt, x, y)                    \
    ui_window_widget(win, (struct ui_widget)        \
        { UI_LABEL, NULL, x, y, 60, 10, txt })

#define ui_textbox(win, txt, x, y, w, h, nxt)       \
    ui_window_widget(win, (struct ui_widget)        \
        { UI_TEXTBOX, nxt, x, y, w, h, txt })

#define ui_separator(win, x, y, w)                  \
    ui_window_widget(win, (struct ui_widget)        \
        { UI_SEPARATOR, NULL, x, y, w, 2, NULL })

#define ui_center   &__ui_center

enum ui_widget_type
{
    UI_LABEL, UI_TEXTBOX, UI_SEPARATOR
};

struct ui_widget_next
{
    bool is_center;
};

struct ui_widget
{
    enum ui_widget_type type;
    const struct ui_widget_next *next;

    uint32_t x, y;
    uint32_t width, height;

    const string text;
};

struct ui_window;

typedef void (*ui_close_fn)(struct ui_window*);
typedef int (*ui_init_fn)(void);

static struct ui_widget_next __ui_center = {
    .is_center = 1
};

int ui_init(ui_init_fn fn);

struct ui_window *ui_new_window(const string name, uint32_t width,
                                uint32_t height);

void ui_window_on_close(struct ui_window *win, ui_close_fn fn);
void ui_window_widget(struct ui_window *win, struct ui_widget widget);
void ui_window_open(struct ui_window *win);

#endif /* UI_UI_H */
