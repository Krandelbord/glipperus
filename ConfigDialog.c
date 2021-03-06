#include <gtk/gtk.h>
#include <string.h>
#include "config.h"

#include "ConfigWidget.h"
#include "ConfigDialog.h"
#include "KonfigPath.h"
#include "HelpWindow.h"

static GtkWidget *config_dialog_draw_common_config(GKeyFile *key_config_file, RuntimeSettings *rts);
static GtkWidget *config_dialog_draw_key_config(GKeyFile *key_config_file);
static GtkWidget *config_dialog_draw_buttonbar(GtkWindow *parent_win);
static void config_dialog_save_data(GtkWidget *config_panel, GKeyFile *keyfile);
static void on_destroy_cb(GtkObject *obj, gpointer data);

//static void config_window_draw_background(GtkWidget *window);

void config_dialog_new(RuntimeSettings *rts) {
	GtkWidget *config_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(config_window), 10);
	
	gtk_window_set_position(GTK_WINDOW(config_window), GTK_WIN_POS_CENTER);
	
	
	gtk_window_set_title(GTK_WINDOW(config_window), _("glipperus - configuration"));
	
	GdkPixbuf *icon_pbxf = gtk_widget_render_icon(config_window, "gtk-preferences", GTK_ICON_SIZE_MENU, NULL);	
	gtk_window_set_icon(GTK_WINDOW(config_window), icon_pbxf);
	g_object_unref(icon_pbxf);
	
	GtkWidget *main_box = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(config_window), main_box);
	
	GtkWidget *notebook = gtk_notebook_new();

	//gtk_container_add(GTK_CONTAINER(config_window), notebook);
	gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), config_dialog_draw_buttonbar(GTK_WINDOW(config_window)), FALSE, FALSE, 0);
	
	GKeyFile *keyfile = g_key_file_new();
	KonfigPath *konf_path = konfig_path_new("glipperus");
	g_key_file_load_from_file(keyfile, konf_path->configuration, G_KEY_FILE_KEEP_COMMENTS, NULL);
	konfig_path_free(konf_path);
	
	GtkWidget *common_conf = config_dialog_draw_common_config(keyfile, rts);
	
	/* We save pointer to Box with all config widget so we can easly get them when closing window */
	g_object_set_data(G_OBJECT(config_window), "config_panel", common_conf);
	g_object_set_data(G_OBJECT(config_window), "keyfile", keyfile);
	g_object_set_data(G_OBJECT(config_window), "RuntimeSettings_pointer", rts);
	
	GtkWidget *common_conf_label = gtk_label_new(_("Common preferences"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), common_conf, common_conf_label);
	
	GtkWidget *key_conf_panel = config_dialog_draw_key_config(keyfile);
	GtkWidget *key_conf_label = gtk_label_new(_("keyboard shortcuts"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), key_conf_panel, key_conf_label);
	
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

static GtkWidget *config_dialog_draw_common_config(GKeyFile *konfig_keyfile, RuntimeSettings *rts) {
	/* Dont mess up with new widget type, because, reading data from 
	 * all this window, and saving to file dpeneds of widget type
	 * So you can easily add more ConfigWidgets here, but It needs more modifications if you want
	 * to add something else 
	 */
	
	GtkWidget *main_box = gtk_vbox_new(FALSE, 10);
	GtkWidget *title_lb = gtk_label_new(NULL);
	gtk_label_set_markup (GTK_LABEL (title_lb), _("<b><big>glipperus - configuration</big></b>"));
	gtk_box_pack_start(GTK_BOX(main_box), title_lb, FALSE, FALSE, 10);
	
	gboolean ov_prim = runtime_settings_get_override_sel(rts);
	GtkWidget *set_01 = config_widget_new_bool(rts, ov_prim, konfig_keyfile, "override_primary", _("Override primary selection"));
	gtk_box_pack_start(GTK_BOX(main_box), set_01, FALSE, FALSE, 0);
	
	gboolean ov_sim = runtime_settings_get_overwrite_similar(rts);
	GtkWidget *set_02 = config_widget_new_bool(rts, ov_sim, konfig_keyfile, "overwrite_similar", _("Overwrite similar entries"));
	gtk_box_pack_start(GTK_BOX(main_box), set_02, FALSE, FALSE, 0);
	
	gboolean transp_tray = runtime_settings_get_trasparent_tray(rts);
	GtkWidget *set_03 = config_widget_new_bool(rts, transp_tray, konfig_keyfile, "transparent_tray", _("Transparent tray icon"));
	gtk_box_pack_start(GTK_BOX(main_box), set_03, FALSE, FALSE, 0);
	
	gint tray_size = runtime_settings_get_tray_icon_size(rts);
	GtkWidget *set_04 = config_widget_new_combo(rts, tray_size, konfig_keyfile, "tray_icon_size", 
		_("Tray icon size"), 
		_("Tiny"), 
		_("Small"), 
		_("Normal"), 
		_("Large"), 
		_("Huge"), 
		_("Extra huge"), 
		NULL);
	gtk_box_pack_start(GTK_BOX(main_box), set_04, FALSE, FALSE, 0);	
	
	gint entries_numb = runtime_settings_get_number_of_entries(rts);
	GtkWidget *set_05 = config_widget_new_int(rts, entries_numb, konfig_keyfile, "number_of_menuitems", _("Number of items in menu"));
	gtk_box_pack_start(GTK_BOX(main_box), set_05, FALSE, FALSE, 0);
	
	gint menu_width = runtime_settings_get_menu_width(rts);
	GtkWidget *set_06 = config_widget_new_int(rts, menu_width, konfig_keyfile, "menu_entry_width", _("Width of menu items in [chars]"));
	gtk_box_pack_start(GTK_BOX(main_box), set_06, FALSE, FALSE, 0);
	
	//gtk_container_foreach(GTK_CONTAINER(main_box), each_widget_cb, konfig_keyfile);
	
	return main_box;
}

static void tg_button_pressed_cb(GtkToggleButton *bt, gpointer data) {
	//gtk_button_set_label(GTK_BUTTON(bt), _("Press key combination..."));
	if  (gtk_toggle_button_get_active(bt)) {
		gtk_button_set_label(GTK_BUTTON(bt), _("Press key combination..."));
		gdk_keyboard_ungrab (GDK_CURRENT_TIME);
	} else {
		gtk_button_set_label(GTK_BUTTON(bt), _("Choose keyboard shortcut"));
		gdk_keyboard_grab(gtk_widget_get_root_window(GTK_WIDGET(bt)), TRUE, GDK_CURRENT_TIME);
	}
	
}

gboolean key_is_not_modifier(gchar *keyname) {
	if (strcmp(keyname, "Alt_L")==0) return FALSE;
	if (strcmp(keyname, "Alt_R")==0) return FALSE;
	
	if (strcmp(keyname, "Control_R")==0) return FALSE;
	if (strcmp(keyname, "Control_L")==0) return FALSE;
	
	if (strcmp(keyname, "Shift_L")==0) return FALSE;
	if (strcmp(keyname, "Shift_R")==0) return FALSE;
	if (strcmp(keyname, "Meta_L")==0) return FALSE;
	if (strcmp(keyname, "ISO_Level3_Shift")==0) return FALSE;
	return TRUE;
}

static void cb_keyb_press(GtkWidget   *widget, GdkEventKey *event, gpointer user_data) {
	glipperus_debug("Wcisnieto klawisz\n");
	guint keyval;
	GdkModifierType cons_modif;
	gdk_keymap_translate_keyboard_state(gdk_keymap_get_default (),
		event->hardware_keycode, event->state, event->group, &keyval, NULL, NULL, &cons_modif);
	gchar *accelerator = gtk_accelerator_name(keyval, event->state);
	
	//returned string should not be modified
	gchar *last_key = gdk_keyval_name(keyval);
	
	/* There must be some real key (not ctrl/alt/shift) to assign shotrcut key */
	if (key_is_not_modifier(last_key)) {
		
		glipperus_debug("Koniec !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		gchar **all_keys = g_strsplit(accelerator, ">", -1);
		if (all_keys !=NULL) {
			int i=0;
			while (all_keys[i]!=NULL) {
				glipperus_debug("[%d]=%s\n", i, all_keys[i]);
				++i;
			}
		}
	}
	glipperus_debug("Wcisnieto klawisz %d hw=%d, accel = %s\n", keyval, event->hardware_keycode, accelerator);
	glipperus_debug("Nazwa tego przyciska = %s\n", gdk_keyval_name(keyval));
	glipperus_debug("Human redable %s \n", gtk_accelerator_get_label(keyval, cons_modif));
}

static GtkWidget *config_dialog_draw_key_config(GKeyFile *konfig_key_file) {
	GtkWidget *main_box = gtk_vbox_new(FALSE, 10);
	GtkWidget *title_lb = gtk_label_new(NULL); 
	gtk_label_set_markup(GTK_LABEL(title_lb), _("<b><big>glipperus key config</big></b>"));
	gtk_box_pack_start(GTK_BOX(main_box), title_lb, FALSE, FALSE, 0);
	
	GtkWidget *button = gtk_toggle_button_new_with_label(_("Choose keyboard shortcut"));
	gtk_box_pack_start(GTK_BOX(main_box), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "toggled", G_CALLBACK(tg_button_pressed_cb), NULL);
	g_signal_connect(G_OBJECT(button), "key-press-event", G_CALLBACK (cb_keyb_press), NULL);
	return main_box;
}

static void on_cancel_clicked(GtkWidget *button, gpointer user_data) {
	glipperus_debug("Cancel clicked\n");
	gtk_widget_destroy(GTK_WIDGET(user_data));
}

static void on_help_clicked(GtkWidget *button, gpointer user_data) {
	GtkWindow *parent_win = GTK_WINDOW(user_data);
	
	GtkWindow *old_help_win = g_object_get_data(G_OBJECT(parent_win), "help window");
	
	if (GTK_IS_WINDOW(old_help_win)) {
		GtkWidget *bt = help_window_get_notify_button(old_help_win);
		gtk_widget_show_all(bt);
		// Let's hide this button after 3 seconds
		g_timeout_add(3*1000, help_window_hide_notify_button, bt); 
	} else {	
		GtkWindow *help_win = help_window_new(parent_win);
		g_object_set_data(G_OBJECT(parent_win), "help window", help_win);
	}
	glipperus_debug("Help clicked\n");
}

static void on_save_clicked(GtkWidget *button, gpointer user_data) {	
	GtkWindow *main_config_window = (GtkWindow*) user_data;
	
	GtkWidget *common_conf_wdg = g_object_get_data(G_OBJECT(main_config_window), "config_panel");
	GKeyFile *keyfile = g_object_get_data(G_OBJECT(main_config_window), "keyfile");
	RuntimeSettings *rts = g_object_get_data(G_OBJECT(main_config_window), "RuntimeSettings_pointer");
	
	config_dialog_save_data(common_conf_wdg, keyfile);
	
	runtime_settings_read_from_file(rts, keyfile);
	
	g_key_file_free(keyfile);
	
	gtk_widget_destroy(GTK_WIDGET(main_config_window));
}

static GtkWidget *config_dialog_draw_buttonbar(GtkWindow *parent_win) {
	GtkWidget *hbtb = gtk_hbutton_box_new();

	GtkWidget *bt_cancel = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_container_add(GTK_CONTAINER(hbtb),bt_cancel);
	g_signal_connect(G_OBJECT(bt_cancel), "clicked", G_CALLBACK(on_cancel_clicked), parent_win);
	
	GtkWidget *bt_help = gtk_button_new_from_stock(GTK_STOCK_HELP);
	gtk_container_add(GTK_CONTAINER(hbtb),bt_help);
	g_signal_connect(G_OBJECT(bt_help), "clicked", G_CALLBACK(on_help_clicked), parent_win);
	
	GtkWidget *bt1 = gtk_button_new_from_stock(GTK_STOCK_APPLY);
	gtk_container_add(GTK_CONTAINER(hbtb),bt1);
	g_signal_connect(G_OBJECT(bt1), "clicked", G_CALLBACK(on_save_clicked), parent_win);
	
	gtk_widget_show_all(hbtb);
	return hbtb;
}

static void on_destroy_cb(GtkObject *obj, gpointer data) {

}

static void config_dialog_save_data(GtkWidget *config_panel, GKeyFile *keyfile) {
	gtk_container_foreach(GTK_CONTAINER(config_panel), each_widget_cb, keyfile);
	gchar *raw_keyfile = g_key_file_to_data(keyfile, NULL, NULL);
	KonfigPath *konf_path = konfig_path_new("glipperus");
	FILE *config_fp = fopen(konf_path->configuration, "w");
	konfig_path_free(konf_path);
	fprintf(config_fp, raw_keyfile);
	fclose(config_fp);
	g_free(raw_keyfile);
}

void config_dialog_free(void) {
	/* this may be useful some day */
	
}
