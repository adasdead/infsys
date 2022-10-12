#ifndef _GUI_H
#define _GUI_H

#include "infsys.h"

#define GUI_INIT_SUCCESS     0
#define GUI_INIT_FAILED      -1

enum widget_type
{
    W_WINDOW, W_LABEL, W_TXTBOX
};

typedef struct _widget_t
{
    size_t type;
    void *self;
    
} widget_t;

typedef struct
{
    int x, y;

} pos_t;

typedef struct _window_t *window_t;
typedef struct _label_t *label_t;
typedef struct _txtbox_t *txtbox_t;

// Window

window_t new_window(cstring_t name, pos_t size);
void window_attach(window_t win, void *widget_p);
void window_on_close(window_t win, void (*close_fn)(window_t));
void window_no_resizable(window_t win);
void window_open(window_t win);

// Label

#define LABEL(win, x, y, text) \
    window_attach(win, new_label(text, (pos_t) { x, y }, (pos_t) { 100, 50 }))

label_t new_label(cstring_t text, pos_t pos, pos_t size);

// Text Box

#define TXTBOX(win, x, y, w, h, text) \
    window_attach(win, new_textbox(text, (pos_t) { x, y }, (pos_t) { w, h }))

txtbox_t new_textbox(cstring_t text, pos_t pos, pos_t size);

// Other

#define GUI_WIDGET_FREE(widget) \
    __gui_free(gui_to_widget(widget));

void gui_init(int (*init_fn)());
widget_t* gui_to_widget(void *widget);
void __gui_free(widget_t *widget);

#endif // _GUI_H