#include "HelpWindow.h"

GtkWidget *help_window_new(GtkWindow *parent_win) {
	GtkWidget *help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW(help_window), parent_win);

	return help_window;
}
