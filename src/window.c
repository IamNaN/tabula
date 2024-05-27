#include "window.h"
#include "events.h"
#include "file_operations.h"
#include "text_editing.h"
#include "toolbar.h"

static guint auto_save_timer = 0;

void on_text_buffer_changed(GtkTextBuffer *buffer, gpointer user_data)
{
  // Update statistics on text buffer change
  update_statistics(buffer);
}

void on_begin_user_action(GtkTextBuffer *buffer, gpointer user_data)
{
  // This function can be used if we need to handle something at the beginning of the user action
}

void on_end_user_action(GtkTextBuffer *buffer, gpointer user_data)
{
  // This function can be used if we need to handle something at the end of the user action
}

void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *scrolled_window;
  GtkWidget *text_view;
  GtkWidget *box;
  GtkWidget *main_box;
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;
  GtkStyleContext *context;

  // Create a new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "tabula");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_fullscreen(GTK_WINDOW(window));

  // Create a new scrolled window
  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_widget_set_vexpand(scrolled_window, TRUE); // Ensure the scrolled window expands vertically

  // Create a new text view
  text_view = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR); // Enable word wrap

  // Set margins to the text view
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view), 20);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view), 20);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 20);  // Set left margin
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view), 20); // Set right margin

  // Set a fixed width for the text view
  gtk_widget_set_size_request(text_view, 760, -1); // 760 pixels to account for the margins

  // Create a box to center the text view horizontally
  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
  gtk_box_pack_start(GTK_BOX(box), text_view, TRUE, FALSE, 0);

  // Create an outer box to center the inner box
  GtkWidget *outer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE);
  gtk_box_pack_start(GTK_BOX(outer_box), box, TRUE, FALSE, 0);

  // Add the outer box to the scrolled window
  gtk_container_add(GTK_CONTAINER(scrolled_window), outer_box);

  // Create the toolbar
  GtkWidget *toolbar = create_toolbar();

  // Create the main box to hold the text view and the toolbar
  main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(main_box), scrolled_window, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(main_box), toolbar, FALSE, FALSE, 0);

  // Create CSS provider and apply styles from file
  provider = gtk_css_provider_new();
  display = gdk_display_get_default();
  screen = gdk_display_get_default_screen(display);
  gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  if (!gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "themes/default.css", NULL))
  {
    g_warning("Failed to load CSS file.");
  }

  // Apply CSS to scrolled window
  context = gtk_widget_get_style_context(scrolled_window);
  gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  // Apply CSS to text view
  context = gtk_widget_get_style_context(text_view);
  gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  g_object_unref(provider);

  // Connect the key press event to the text view
  g_signal_connect(G_OBJECT(text_view), "key-press-event", G_CALLBACK(on_key_press), toolbar);

  // Connect the buffer changed event to start auto-save and capture text changes
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  g_signal_connect(buffer, "changed", G_CALLBACK(on_text_buffer_changed), text_view);
  g_signal_connect(buffer, "begin-user-action", G_CALLBACK(on_begin_user_action), NULL);
  g_signal_connect(buffer, "end-user-action", G_CALLBACK(on_end_user_action), NULL);

  // Try to open the auto-save file on startup
  try_open_auto_save(GTK_TEXT_VIEW(text_view));

  // Add the initial state to the undo stack
  push_undo(buffer);

  // Update statistics initially
  update_statistics(buffer);

  // Add the main box to the main window
  gtk_container_add(GTK_CONTAINER(window), main_box);

  // Show all widgets
  gtk_widget_show_all(window);
}
