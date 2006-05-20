#include <gtk/gtk.h>
#include "config.h"

static GtkWidget *config_dialog_draw_common_config(void);
static GtkWidget *config_dialog_draw_key_config(void);
static void on_destroy_cb(GtkObject *obj, gpointer data);

void config_dialog_new(void) {
	GtkWidget *config_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(config_window), "destroy", G_CALLBACK(on_destroy_cb), NULL);
	gtk_window_set_position(GTK_WINDOW(config_window), GTK_WIN_POS_CENTER);
	
	gtk_window_set_title(GTK_WINDOW(config_window), _("glipper - configuration"));
	
	GdkPixbuf *icon_pbxf = gtk_widget_render_icon(config_window, "gtk-preferences", GTK_ICON_SIZE_MENU, NULL);	
	gtk_window_set_icon(GTK_WINDOW(config_window), icon_pbxf);
	
	GtkWidget *notebook = gtk_notebook_new();
	gtk_container_add(GTK_CONTAINER(config_window), notebook);
	
	GtkWidget *common_conf = config_dialog_draw_common_config();
	GtkWidget *common_conf_label = gtk_label_new(_("Common preferences"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), common_conf, common_conf_label);
	
	GtkWidget *key_conf = config_dialog_draw_key_config();
	GtkWidget *key_conf_label = gtk_label_new(_("keyboard shortcuts"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), key_conf, key_conf_label);
	
	gtk_widget_show_all(config_window);
}

static GtkWidget *config_dialog_draw_common_config(void) {
	GtkWidget *main_box = gtk_vbox_new(FALSE, 0);
	GtkWidget *title_lb = gtk_label_new(_("glipper settings"));
	gtk_box_pack_start(GTK_BOX(main_box), title_lb, FALSE, FALSE, 0);
	
	return main_box;
}

static GtkWidget *config_dialog_draw_key_config(void) {
	GtkWidget *main_box = gtk_vbox_new(FALSE, 0);
	GtkWidget *title_lb = gtk_label_new(_("glipper key config"));
	gtk_box_pack_start(GTK_BOX(main_box), title_lb, FALSE, FALSE, 0);
	
	return main_box;
}

static void on_destroy_cb(GtkObject *obj, gpointer data) {
	gtk_main_quit();
}

void config_dialog_free(void) {
	/* we need to unref the icon_pbxf */
	
}
