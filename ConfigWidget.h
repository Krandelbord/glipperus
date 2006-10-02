#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include <gtk/gtk.h>
#include "RuntimeSettings.h"

GtkWidget *config_widget_new_bool(RuntimeSettings *rts, gboolean value, GKeyFile *keyfile, gchar *key, gchar *title);
GtkWidget *config_widget_new_int(RuntimeSettings *rts, gint value, GKeyFile *keyfile, gchar *key, gchar *title);

/* NULL terminated list of combo_box entries  */
GtkWidget *config_widget_new_combo(RuntimeSettings *rts, gint value, GKeyFile *keyfile, gchar *key, gchar *title, gchar *element, ...);

void config_widget_read_data_from_widgets(GtkWidget *config_widget, gpointer data);

#endif
