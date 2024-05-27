#include "text_editing.h"
#include <glib.h>
#include <stdio.h>
#include <ctype.h>

// Undo/redo stack
typedef struct
{
  gchar *text;
  gint start;
  gint end;
  gint cursor_pos;
} TextChange;

static GList *undo_stack = NULL;
static GList *redo_stack = NULL;
static gboolean is_undo_or_redo = FALSE;

void push_undo(GtkTextBuffer *buffer)
{
  if (is_undo_or_redo)
    return;

  GtkTextIter start, end, cursor;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  gtk_text_buffer_get_iter_at_mark(buffer, &cursor, gtk_text_buffer_get_insert(buffer));

  TextChange *change = g_new(TextChange, 1);
  change->text = g_strdup(gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
  change->start = gtk_text_iter_get_offset(&start);
  change->end = gtk_text_iter_get_offset(&end);
  change->cursor_pos = gtk_text_iter_get_offset(&cursor);

  undo_stack = g_list_prepend(undo_stack, change);
  g_list_free_full(redo_stack, g_free);
  redo_stack = NULL;

  // Debug log
  // g_print("Push undo: %s\n", change->text);
}

void undo(GtkTextView *text_view)
{
  if (undo_stack == NULL)
    return;

  is_undo_or_redo = TRUE;

  TextChange *change = undo_stack->data;
  undo_stack = g_list_remove(undo_stack, change);

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter start, end, cursor;
  gtk_text_buffer_get_iter_at_offset(buffer, &start, change->start);
  gtk_text_buffer_get_iter_at_offset(buffer, &end, change->end);
  gtk_text_buffer_set_text(buffer, change->text, -1);
  gtk_text_buffer_get_iter_at_offset(buffer, &cursor, change->cursor_pos);
  gtk_text_buffer_place_cursor(buffer, &cursor);

  redo_stack = g_list_prepend(redo_stack, change);

  // Debug log
  // g_print("Undo: %s\n", change->text);

  is_undo_or_redo = FALSE;
}

void redo(GtkTextView *text_view)
{
  if (redo_stack == NULL)
    return;

  is_undo_or_redo = TRUE;

  TextChange *change = redo_stack->data;
  redo_stack = g_list_remove(redo_stack, change);

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter start, end, cursor;
  gtk_text_buffer_get_iter_at_offset(buffer, &start, change->start);
  gtk_text_buffer_get_iter_at_offset(buffer, &end, change->end);
  gtk_text_buffer_set_text(buffer, change->text, -1);
  gtk_text_buffer_get_iter_at_offset(buffer, &cursor, change->cursor_pos);
  gtk_text_buffer_place_cursor(buffer, &cursor);

  undo_stack = g_list_prepend(undo_stack, change);

  // Debug log
  // g_print("Redo: %s\n", change->text);

  is_undo_or_redo = FALSE;
}

void clear_undo_redo_stacks()
{
  g_list_free_full(undo_stack, g_free);
  g_list_free_full(redo_stack, g_free);
  undo_stack = NULL;
  redo_stack = NULL;
}
