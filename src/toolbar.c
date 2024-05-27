#include "toolbar.h"
#include <gtk/gtk.h>
#include <stdlib.h>

#define WORDS_PER_PAGE 390 // Define the number of words per page

GtkWidget *file_name_label;
GtkWidget *page_count_label;
GtkWidget *word_count_label;

GtkWidget *create_toolbar()
{
  GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_size_request(toolbar, 800, 30);

  // Create labels for file name, page, and word counts
  file_name_label = gtk_label_new("[Untitled]");
  page_count_label = gtk_label_new("Pages: 0");
  word_count_label = gtk_label_new("Words: 0");

  // Add the file name label to the left
  gtk_box_pack_start(GTK_BOX(toolbar), file_name_label, FALSE, FALSE, 10);

  // Add a spacer to push the page and word count labels to the right
  GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, TRUE);
  gtk_box_pack_start(GTK_BOX(toolbar), spacer, TRUE, TRUE, 0);

  // Add the page and word count labels to the toolbar
  gtk_box_pack_start(GTK_BOX(toolbar), page_count_label, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(toolbar), word_count_label, FALSE, FALSE, 10);

  return toolbar;
}

void update_statistics(GtkTextBuffer *buffer)
{
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);

  gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  if (text)
  {
    int word_count = 0;

    // Count words
    for (gchar *p = text; *p; p++)
    {
      if (g_unichar_isspace(g_utf8_get_char(p)) && (p == text || !g_unichar_isspace(g_utf8_get_char(p - 1))))
      {
        word_count++;
      }
    }

    // Calculate page count based on word count and words per page
    int page_count = (word_count + WORDS_PER_PAGE - 1) / WORDS_PER_PAGE;

    // Update labels
    gchar *page_count_text = g_strdup_printf("Pages: %d", page_count);
    gchar *word_count_text = g_strdup_printf("Words: %d", word_count);

    gtk_label_set_text(GTK_LABEL(page_count_label), page_count_text);
    gtk_label_set_text(GTK_LABEL(word_count_label), word_count_text);

    g_free(page_count_text);
    g_free(word_count_text);
  }
  g_free(text);
}

void update_file_name(const gchar *file_name)
{
  if (file_name == NULL || g_strcmp0(file_name, "") == 0)
  {
    gtk_label_set_text(GTK_LABEL(file_name_label), "[Untitled]");
  }
  else
  {
    gtk_label_set_text(GTK_LABEL(file_name_label), file_name);
  }
}
