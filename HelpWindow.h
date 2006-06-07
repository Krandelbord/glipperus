#include <gtk/gtk.h>

GtkWindow *help_window_new(GtkWindow *parent_win);
GtkWidget *help_window_get_notify_button(GtkWindow *help_window);
gboolean help_window_hide_notify_button(gpointer gp_button);
