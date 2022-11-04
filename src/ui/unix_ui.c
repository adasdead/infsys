#ifdef __unix__

#include "ui/ui.h"

#include <gtk/gtk.h>

#define APPLICATION_ID "com.adasdead.infsys"

struct ui_window
{
    GtkWidget *win;
    GtkWidget *fixed;

    ui_close_fn close_fn;
};

static GtkApplication *g_app;

static void ui_resize_font(enum ui_widget_type type,
                           GtkWidget *widget)
{
    PangoFontDescription *desc;
    desc = pango_font_description_from_string("Monospace 9");

    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gtk_widget_override_font(widget, desc);
    G_GNUC_END_IGNORE_DEPRECATIONS

    pango_font_description_free(desc);
}

static int ui_activate(GtkApplication *app, gpointer data)
{
    return ((ui_init_fn) data)();
}

static gboolean ui_window_deleted(GtkWidget *widget,
                                  GdkEvent *event,
                                  gpointer data)
{
    struct ui_window *win = (struct ui_window*) data;
    if (win->close_fn) win->close_fn(win);
    free(win);

    return 0;
}

int ui_init(ui_init_fn fn)
{
    int status;

    g_app = gtk_application_new(APPLICATION_ID,
                                G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(g_app, "activate", G_CALLBACK(ui_activate), fn);
    status = g_application_run(G_APPLICATION(g_app), 0, NULL);
    g_object_unref(g_app);

    return status;
}

struct ui_window *ui_new_window(const string name, uint32_t width,
                                uint32_t height)
{
    struct ui_window *tmp = malloc(sizeof(*tmp));

    tmp->win = gtk_application_window_new(g_app);
    tmp->fixed = gtk_fixed_new();
    tmp->close_fn = NULL;

    g_signal_connect(G_OBJECT(tmp->win), "delete-event",
                     G_CALLBACK(ui_window_deleted), tmp);

    gtk_window_set_title(GTK_WINDOW(tmp->win), name);
    gtk_window_set_default_size(GTK_WINDOW(tmp->win), width, height);
    gtk_window_set_resizable(GTK_WINDOW(tmp->win), 0);

    gtk_container_add(GTK_CONTAINER(tmp->win), tmp->fixed);

    return tmp;
}

void ui_window_on_close(struct ui_window *win, ui_close_fn fn)
{
    win->close_fn = fn;
}

void ui_window_widget(struct ui_window *win, struct ui_widget widget)
{
    GtkWidget *tmp;
    GtkTextBuffer *buf;

    switch (widget.type)
    {
    case UI_LABEL:
        tmp = gtk_label_new(widget.text);
        widget.x -= 45;
        widget.y -= 17;
        break;

    case UI_TEXTBOX:
        tmp = gtk_text_view_new();
        buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tmp));
        gtk_text_buffer_set_text(buf, widget.text, strlen(widget.text));
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tmp), GTK_WRAP_CHAR);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(tmp), 0);
        break;

    case UI_SEPARATOR:
        tmp = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        break;
    }

    ui_resize_font(widget.type, tmp);

    gtk_widget_set_size_request(tmp, widget.width, widget.height);
    gtk_fixed_put(GTK_FIXED(win->fixed), tmp, widget.x, widget.y);
}

void ui_window_open(struct ui_window *win)
{
    gtk_widget_show_all(win->win);
}

#endif /* __unix__ */
