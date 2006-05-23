#include <gtk/gtk.h>
#include "KonfigPath.h"

#include "ConfigWidget.h"
#include "config.h"

static GtkWidget *config_dialog_draw_common_config(GKeyFile *key_config_file);
static GtkWidget *config_dialog_draw_key_config(GKeyFile *key_config_file);
static void on_destroy_cb(GtkObject *obj, gpointer data);

//static void config_window_draw_background(GtkWidget *window);

void config_dialog_new(void) {
	GtkWidget *config_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(config_window), 10);
	
	gtk_window_set_position(GTK_WINDOW(config_window), GTK_WIN_POS_CENTER);
	
	
	gtk_window_set_title(GTK_WINDOW(config_window), _("glipper - configuration"));
	
	GdkPixbuf *icon_pbxf = gtk_widget_render_icon(config_window, "gtk-preferences", GTK_ICON_SIZE_MENU, NULL);	
	gtk_window_set_icon(GTK_WINDOW(config_window), icon_pbxf);
	g_object_unref(icon_pbxf);
	
	GtkWidget *notebook = gtk_notebook_new();

	gtk_container_add(GTK_CONTAINER(config_window), notebook);
	
	GKeyFile *keyfile = g_key_file_new();
	KonfigPath *konf_path = konfig_path_new("glipper");
	g_key_file_load_from_file(keyfile, konf_path->configuration, G_KEY_FILE_KEEP_COMMENTS, NULL);
	konfig_path_free(konf_path);
	
	GtkWidget *common_conf = config_dialog_draw_common_config(keyfile);
	
	/* We save pointer to Box with all config widget so we can easly get them when closing window */
	g_object_set_data(G_OBJECT(config_window), "config_panel", common_conf);
	
	GtkWidget *common_conf_label = gtk_label_new(_("Common preferences"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), common_conf, common_conf_label);
	
	GtkWidget *key_conf = config_dialog_draw_key_config(keyfile);
	GtkWidget *key_conf_label = gtk_label_new(_("keyboard shortcuts"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), key_conf, key_conf_label);
	
	gtk_widget_show_all(config_window);
	g_signal_connect(G_OBJECT(config_window), "destroy", G_CALLBACK(on_destroy_cb), keyfile);
}

/*static void config_window_draw_background(GtkWidget *window) {
	GdkPixbuf *pref_image_pxbf = gtk_widget_render_icon(window, "gtk-preferences", GTK_ICON_SIZE_DIALOG, NULL);
	GdkPixmap *pixmap;
	GdkBitmap *maska;
	gdk_pixbuf_render_pixmap_and_mask(pref_image_pxbf, &pixmap, &maska, 0);
	gtk_widget_set_app_paintable(window, FALSE);
	gtk_widget_realize(window);
	gdk_window_set_back_pixmap(window->window, pixmap, FALSE);
	//gdk_window_set_back_pixmap(gtk_widget_get_root_window(config_window), pixmap, FALSE);
}*/

void each_widget_cb(GtkWidget *widget, gpointer data) {
	GKeyFile *keyfile = (GKeyFile*) data;
	if (!GTK_IS_LABEL(widget)) {
		config_widget_read_data_from_widgets(widget, keyfile);
	}
}

static GtkWidget *config_dialog_draw_common_config(GKeyFile *konfig_keyfile) {
	/* Dont mess up with new widget type, because, reading data from 
	 * all this window, and saving to file dpeneds of widget type
	 * So you can easily add ConfigWidgets here, but It needs more modifications if you want
	 * to add something else 
	 */
	
	GtkWidget *main_box = gtk_vbox_new(FALSE, 10);
	GtkWidget *title_lb = gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL (title_lb), _("<b><big>glipper - configuration</big></b>"));
	gtk_box_pack_start(GTK_BOX(main_box), title_lb, FALSE, FALSE, 10);
	
	GtkWidget *set_01 = config_widget_new_bool(konfig_keyfile, "override primary", _("Override primary selection"));
	gtk_box_pack_start(GTK_BOX(main_box), set_01, FALSE, FALSE, 0);
	
	GtkWidget *set_02 = config_widget_new_bool(konfig_keyfile, "transparent tray", _("Transparent tray icon"));
	gtk_box_pack_start(GTK_BOX(main_box), set_02, FALSE, FALSE, 0);
	
	GtkWidget *set_05 = config_widget_new_combo(konfig_keyfile, "tray icon size", _("Tray icon size"), _("Small"), _("Medium"), _("Large"), NULL);
	gtk_box_pack_start(GTK_BOX(main_box), set_05, FALSE, FALSE, 0);	
	
	GtkWidget *set_03 = config_widget_new_int(konfig_keyfile, "number of menuitems", _("Number of items in menu"));
	gtk_box_pack_start(GTK_BOX(main_box), set_03, FALSE, FALSE, 0);
	
	GtkWidget *set_04 = config_widget_new_int(konfig_keyfile, "menu entry width", _("Width of menu items in [chars]"));
	gtk_box_pack_start(GTK_BOX(main_box), set_04, FALSE, FALSE, 0);
	
	//gtk_container_foreach(GTK_CONTAINER(main_box), each_widget_cb, konfig_keyfile);
	
	return main_box;
}

static GtkWidget *config_dialog_draw_key_config(GKeyFile *konfig_key_file) {
	GtkWidget *main_box = gtk_vbox_new(FALSE, 0);
	GtkWidget *title_lb = gtk_label_new(NULL); 
	gtk_label_set_markup(GTK_LABEL(title_lb), _("<b><big>glipper key config</big></b>"));
	gtk_box_pack_start(GTK_BOX(main_box), title_lb, FALSE, FALSE, 0);
	
	return main_box;
}

static void on_destroy_cb(GtkObject *obj, gpointer data) {
	GKeyFile *keyfile = (GKeyFile *) data;
	GtkWidget *common_conf_wdg = g_object_get_data(G_OBJECT(obj), "config_panel");
	gtk_container_foreach(GTK_CONTAINER(common_conf_wdg), each_widget_cb, keyfile);
	gchar *raw_keyfile = g_key_file_to_data(keyfile, NULL, NULL);
	KonfigPath *konf_path = konfig_path_new("glipper");
	FILE *config_fp = fopen(konf_path->configuration, "w");
	konfig_path_free(konf_path);
	fprintf(config_fp, raw_keyfile);
	fclose(config_fp);
	g_free(raw_keyfile);
	g_key_file_free(keyfile);
	gtk_main_quit();
}

void config_dialog_free(void) {
	/* we need to unref the icon_pbxf */
	
}
