#include "ConfigWidget.h"
void on_combo_changed(GtkComboBox *combo, gpointer data) {
	g_print("Wybrano elemenet %d \n", gtk_combo_box_get_active(combo));	
}

/* Creates hbox, and puts label for it. Returns createad hbox */
static GtkWidget *config_widget_common(gchar *label_txt);


GtkWidget *config_widget_new_bool(gchar *key) {
	GtkWidget *chck_button = gtk_check_button_new_with_label(key);
	
	return chck_button;
}

GtkWidget *config_widget_new_int(gchar *key) {
	GtkWidget *common = config_widget_common(key);
	GtkWidget *spin_btn = gtk_spin_button_new_with_range(1, 99, 1);
	gtk_box_pack_start(GTK_BOX(common), spin_btn, FALSE, FALSE, 0);
	return common;
}

GtkWidget *config_widget_new_combo(gchar *title, gchar *element, ...) {
	va_list args;
	va_start(args, element);
	GtkWidget *common = config_widget_common(title);
	
	GtkWidget *combo = gtk_combo_box_new_text();
	gtk_box_pack_start(GTK_BOX(common), combo, FALSE, FALSE, 0);
	
	if (element) gtk_combo_box_append_text(GTK_COMBO_BOX(combo), element);
	gchar *elem;
	while ( (elem= va_arg(args, gchar*)) ) 
		gtk_combo_box_append_text(GTK_COMBO_BOX(combo), elem);
	
	va_end(args);
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
	g_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(on_combo_changed), NULL);
	return common;
}

static GtkWidget *config_widget_common(gchar *label_txt) {
	GtkWidget *main_box = gtk_hbox_new(FALSE, 0);
	
	GtkWidget *label = gtk_label_new(label_txt);
	gtk_box_pack_start(GTK_BOX(main_box), label, FALSE, FALSE, 10);
	
	return main_box;
}
