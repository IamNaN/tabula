#include "file_operations.h"
#include <glib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

static gchar *current_file = NULL;

void ensure_documents_folder_exists()
{
  const char *home_dir = g_get_home_dir();
  char *documents_folder = g_build_filename(home_dir, "Documents", NULL);

  struct stat st = {0};
  if (stat(documents_folder, &st) == -1)
  {
    mkdir(documents_folder, 0700);
  }

  g_free(documents_folder);
}

void on_save_as(GtkTextView *text_view)
{
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  char *content;

  // Ensure the Documents folder exists
  ensure_documents_folder_exists();

  // Create a new file chooser dialog
  dialog = gtk_file_chooser_dialog_new("Save File As",
                                       GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(text_view))),
                                       GTK_FILE_CHOOSER_ACTION_SAVE,
                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                       "_Save", GTK_RESPONSE_ACCEPT,
                                       NULL);
  chooser = GTK_FILE_CHOOSER(dialog);
  gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);

  // Set the default folder to ~/Documents
  char *documents_folder = g_build_filename(g_get_home_dir(), "Documents", NULL);
  gtk_file_chooser_set_current_folder(chooser, documents_folder);
  g_free(documents_folder);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;

    filename = gtk_file_chooser_get_filename(chooser);

    // Get the text buffer content
    buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Write the content to the file
    g_file_set_contents(filename, content, -1, NULL);

    // Free the filename and content
    g_free(current_file);              // Free the previous filename
    current_file = g_strdup(filename); // Update the current file name
    g_free(filename);
    g_free(content);

    // Remove the temporary auto-save file
    remove(get_auto_save_file_path());
  }

  gtk_widget_destroy(dialog);
}

void on_save(GtkTextView *text_view)
{
  if (current_file == NULL || g_strcmp0(current_file, get_auto_save_file_path()) == 0)
  {
    on_save_as(text_view);
  }
  else
  {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter start, end;
    char *content;

    // Get the text buffer content
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Write the content to the current file
    g_file_set_contents(current_file, content, -1, NULL);

    // Free the content
    g_free(content);

    // Remove the temporary auto-save file
    remove(get_auto_save_file_path());
  }
}

void on_open_file(GtkTextView *text_view)
{
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkTextBuffer *buffer;
  gchar *content;
  gsize length;
  GError *error = NULL;

  // Ensure the Documents folder exists
  ensure_documents_folder_exists();

  // Create a new file chooser dialog
  dialog = gtk_file_chooser_dialog_new("Open File",
                                       GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(text_view))),
                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                       "_Open", GTK_RESPONSE_ACCEPT,
                                       NULL);
  chooser = GTK_FILE_CHOOSER(dialog);

  // Set the default folder to ~/Documents
  char *documents_folder = g_build_filename(g_get_home_dir(), "Documents", NULL);
  gtk_file_chooser_set_current_folder(chooser, documents_folder);
  g_free(documents_folder);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    char *filename;

    filename = gtk_file_chooser_get_filename(chooser);

    // Read the content from the file
    if (g_file_get_contents(filename, &content, &length, &error))
    {
      // Get the text buffer and set its content
      buffer = gtk_text_view_get_buffer(text_view);
      gtk_text_buffer_set_text(buffer, content, length);

      // Free the previous filename and set the new current file
      g_free(current_file);
      current_file = g_strdup(filename);

      // Free the content
      g_free(content);
    }
    else
    {
      g_warning("Could not open file: %s", error->message);
      g_clear_error(&error);
    }

    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}

gboolean auto_save(gpointer user_data)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(user_data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter start, end;
  char *content;
  char *filename = get_auto_save_file_path();

  // Get the text buffer content
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  // Write the content to the auto-save file
  g_file_set_contents(filename, content, -1, NULL);

  // Free the filename and content
  g_free(filename);
  g_free(content);

  return TRUE; // Continue calling this function
}

char *get_auto_save_file_path()
{
  const char *home_dir = g_get_home_dir();
  char *filename = g_build_filename(home_dir, ".tabula_autosave.txt", NULL);
  return filename;
}

void try_open_auto_save(GtkTextView *text_view)
{
  gchar *content;
  gsize length;
  GError *error = NULL;
  char *filename = get_auto_save_file_path();

  // Try to open the auto-save file
  if (g_file_get_contents(filename, &content, &length, &error))
  {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_set_text(buffer, content, length);

    // Free the previous filename and set the new current file
    g_free(current_file);
    current_file = g_strdup(filename);

    // Free the content
    g_free(content);
  }
  else
  {
    g_clear_error(&error);
  }

  g_free(filename);
}
