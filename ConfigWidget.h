#include <gtk/gtk.h>

#ifndef RUNTIME_SETTINGS_H
	#include "RuntimeSettings.h"
#endif

GtkWidget *config_widget_new_bool(RuntimeSettings *rts, gboolean value, GKeyFile *keyfile, gchar *key, gchar *title);
GtkWidget *config_widget_new_int(RuntimeSettings *rts, gint value, GKeyFile *keyfile, gchar *key, gchar *title);

/* NULL terminated list of combo_box entries  */
GtkWidget *config_widget_new_combo(RuntimeSettings *rts, gint value, GKeyFile *keyfile, gchar *key, gchar *title, gchar *element, ...);

void config_widget_read_data_from_widgets(GtkWidget *config_widget, gpointer data);
