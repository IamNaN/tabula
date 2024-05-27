#ifndef TEXT_EDITING_H
#define TEXT_EDITING_H

#include <gtk/gtk.h>

void push_undo(GtkTextBuffer *buffer);
void undo(GtkTextView *text_view);
void redo(GtkTextView *text_view);
void clear_undo_redo_stacks();

#endif // TEXT_EDITING_H
