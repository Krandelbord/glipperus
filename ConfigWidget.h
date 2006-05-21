#include <gtk/gtk.h>

GtkWidget *config_widget_new_bool(gchar *key);
GtkWidget *config_widget_new_int(gchar *key);
GtkWidget *config_widget_new_combo(gchar *title, gchar *element, ...);
