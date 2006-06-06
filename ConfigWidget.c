#include "ConfigWidget.h"

/* Creates hbox, and puts label for it. Returns createad hbox */
static GtkWidget *config_widget_common(gchar *label_txt);

GtkWidget *config_widget_new_bool(RuntimeSettings *rts, gboolean value, GKeyFile *keyfile, gchar *key, gchar *title) {	
	GtkWidget *chck_button = gtk_check_button_new_with_label(title);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chck_button), value);
	
	g_object_set_data(G_OBJECT(chck_button), "key", key);
	return chck_button;
}

GtkWidget *config_widget_new_int(RuntimeSettings *rts, gint value, GKeyFile *keyfile, gchar *key, gchar *title) {
	GtkWidget *common = config_widget_common(title);
	GtkWidget *spin_btn = gtk_spin_button_new_with_range(1, 99, 1);
	gtk_box_pack_start(GTK_BOX(common), spin_btn, FALSE, FALSE, 0);
	
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_btn), value);
	
	g_object_set_data(G_OBJECT(spin_btn), "key", key);
	return common;
}

GtkWidget *config_widget_new_combo(RuntimeSettings *rts, gint value, GKeyFile *keyfile, gchar *key, gchar *title, gchar *element, ...) {
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
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), value);
	
	g_object_set_data(G_OBJECT(combo), "key", key);
	return common;
}

static GtkWidget *config_widget_common(gchar *label_txt) {
	GtkWidget *main_box = gtk_hbox_new(FALSE, 0);
	
	GtkWidget *label = gtk_label_new(label_txt);
	gtk_box_pack_start(GTK_BOX(main_box), label, FALSE, FALSE, 10);
	
	return main_box;
}

void config_widget_read_data_from_widgets(GtkWidget *config_widget, gpointer data) {
	GKeyFile *keyfile = (GKeyFile*) data;
	gchar *key = g_object_get_data(G_OBJECT(config_widget), "key");
	if (GTK_IS_CHECK_BUTTON(config_widget)) {
		gboolean read_from_wdg = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(config_widget));
		//gchar *key = g_object_get_data(G_OBJECT(config_widget), "key");
		g_key_file_set_boolean(keyfile, "main", key, read_from_wdg);
	} else if (GTK_IS_COMBO_BOX(config_widget)) {
		gint read_from_wdg = gtk_combo_box_get_active(GTK_COMBO_BOX(config_widget));
		g_key_file_set_integer(keyfile, "main", key, read_from_wdg);
	} else if (GTK_IS_SPIN_BUTTON(config_widget)) {
		gint read_from_wdg = gtk_spin_button_get_value(GTK_SPIN_BUTTON(config_widget));
		g_key_file_set_integer(keyfile, "main", key, read_from_wdg);
	} else if (GTK_IS_BOX(config_widget)) {
		/* i od nowa polska ludowa... */
		gtk_container_foreach(GTK_CONTAINER(config_widget), config_widget_read_data_from_widgets, keyfile);
	}
}
