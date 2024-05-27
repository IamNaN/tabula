#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <gtk/gtk.h>

GtkWidget *create_toolbar();
void update_statistics(GtkTextBuffer *buffer);
void update_file_name(const gchar *file_name); // Add this line

#endif // TOOLBAR_H
