#include <gtk/gtk.h>
#include "window.h"

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

#ifdef G_APPLICATION_DEFAULT_FLAGS
  app = gtk_application_new("com.example.tabula", G_APPLICATION_DEFAULT_FLAGS);
#else
  app = gtk_application_new("com.example.tabula", 0);
#endif

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
