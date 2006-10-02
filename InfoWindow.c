#include "InfoWindow.h"

#include "config.h"

void info_window_new(void) {
	GtkWidget *info_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(info_window), vbox);

	GtkWidget *label = gtk_label_new(_("glipperus - clipboard manager"));
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);


	gtk_widget_show_all(info_window);
}
