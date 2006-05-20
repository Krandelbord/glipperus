#include <gtk/gtk.h>
#include "config.h"

void config_dialog_new(void) {
	GtkWidget *config_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(config_window), GTK_WIN_POS_CENTER);
	
	gtk_window_set_title(GTK_WINDOW(config_window), _("glipper - configuration"));
	
	gtk_widget_show_all(config_window);	
}
