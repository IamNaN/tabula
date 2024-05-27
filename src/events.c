#include "events.h"
#include "file_operations.h"
#include "text_editing.h"
#include "toolbar.h"
#include <ctype.h>

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));

  // Handle keyboard shortcuts
  if ((event->state & GDK_CONTROL_MASK))
  {
    switch (event->keyval)
    {
    case GDK_KEY_q: // Ctrl+Q to quit
      gtk_widget_destroy(gtk_widget_get_toplevel(widget));
      return TRUE;
    case GDK_KEY_o: // Ctrl+O to open file
      on_open_file(GTK_TEXT_VIEW(widget));
      return TRUE;
    case GDK_KEY_c: // Ctrl+C to copy
      gtk_text_buffer_copy_clipboard(buffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
      return TRUE;
    case GDK_KEY_v: // Ctrl+V to paste
      gtk_text_buffer_paste_clipboard(buffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), NULL, TRUE);
      return TRUE;
    case GDK_KEY_x: // Ctrl+X to cut
      gtk_text_buffer_cut_clipboard(buffer, gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), TRUE);
      return TRUE;
    case GDK_KEY_s: // Ctrl+S to save
      on_save(GTK_TEXT_VIEW(widget));
      return TRUE;
    case GDK_KEY_S: // Ctrl+Shift+S to save as
      on_save_as(GTK_TEXT_VIEW(widget));
      return TRUE;
    case GDK_KEY_z: // Ctrl+Z to undo
      undo(GTK_TEXT_VIEW(widget));
      return TRUE;
    case GDK_KEY_y: // Ctrl+Y to redo
    case GDK_KEY_Z: // Ctrl+Shift+Z to redo
      redo(GTK_TEXT_VIEW(widget));
      return TRUE;
    case GDK_KEY_f: // Ctrl+F to find
      // Add find functionality here
      return TRUE;
    case GDK_KEY_h: // Ctrl+H to replace
      // Add replace functionality here
      return TRUE;
    case GDK_KEY_d: // Ctrl+D to toggle toolbar
      // toggle_toolbar();
      return TRUE;
    }
  }
  else
  {
    // Handle word boundaries for undo stack
    if (event->keyval == GDK_KEY_space || ispunct(event->keyval) || event->keyval == GDK_KEY_Return)
    {
      push_undo(buffer);
    }
  }

  return FALSE;
}
