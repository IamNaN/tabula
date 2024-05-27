#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <gtk/gtk.h>

void on_save(GtkTextView *text_view);
void on_save_as(GtkTextView *text_view);
void on_open_file(GtkTextView *text_view);
gboolean auto_save(gpointer user_data);
char *get_auto_save_file_path();
void try_open_auto_save(GtkTextView *text_view);

#endif // FILE_OPERATIONS_H
