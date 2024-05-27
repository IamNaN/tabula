#ifndef EVENTS_H
#define EVENTS_H

#include <gtk/gtk.h>

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void on_save_as(GtkTextView *text_view);
void on_open_file(GtkTextView *text_view);

#endif // EVENTS_H
