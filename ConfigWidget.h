#include <gtk/gtk.h>

GtkWidget *config_widget_new_bool(GKeyFile *keyfile, gchar *key, gchar *title);
GtkWidget *config_widget_new_int(GKeyFile *keyfile, gchar *key, gchar *title);

/* NULL terminated list of combo_box entries  */
GtkWidget *config_widget_new_combo(GKeyFile *keyfile, gchar *key, gchar *title, gchar *element, ...);

void config_widget_read_data_from_widgets(GtkWidget *config_widget, gpointer data);
