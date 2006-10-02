#include "menu.h"
#include "clipboard.h"
#include "stored_items.h"
#include "InfoWindow.h"
#include "ConfigDialog.h"

#include "config.h"

extern GList *glipperus_stored_items;

static void on_menuitem_clicked (GtkImageMenuItem *menuitem, gpointer oneContextMenu) {
	RuntimeSettings *rts = g_object_get_data(G_OBJECT(menuitem), "RuntimeSettings_pointer");
	
	
	GtkWidget *label = gtk_bin_get_child(GTK_BIN(menuitem));
	const gchar *txt = gtk_label_get_text(GTK_LABEL(label));

	glipperus_stored_items_set_active(txt);
	//glipperus_clipboard_set_txt(txt);
	gtk_clipboard_set_text(gtk_clipboard_get (GDK_SELECTION_CLIPBOARD), txt, -1);
	
	if (runtime_settings_get_override_sel(rts))
		gtk_clipboard_set_text(gtk_clipboard_get (GDK_SELECTION_PRIMARY), txt, -1);
	
	gtk_widget_destroy(GTK_WIDGET(oneContextMenu));
}

static void on_info_clicked(GtkMenuItem *menuitem, gpointer user_data) {
	info_window_new();
}

static void on_preferences_clicked(GtkMenuItem *menuitem, gpointer user_data) {
	RuntimeSettings *rts = (RuntimeSettings*) user_data;
	config_dialog_new(rts);
}

static void on_exit_clicked (GtkMenuItem *menuitem, gpointer user_data) {
	g_print("asta la pasta... bejbe\n");
	gtk_main_quit();
}

static void glipperus_contextMenu_add_clips (gpointer data, gpointer user_data) {
	GtkWidget *contextMenu = user_data;
	glipperus_clip_item *clip = data;
	GtkWidget *clip_content_menu;
	GtkLabel *label;

	RuntimeSettings *rts = g_object_get_data(G_OBJECT(contextMenu), "RuntimeSettings_pointer");
	gint menu_width = runtime_settings_get_menu_width(rts);
	
	if (clip != NULL) {
		clip_content_menu = gtk_image_menu_item_new_with_label (clip->contents);
		
		g_object_set_data(G_OBJECT(clip_content_menu), "RuntimeSettings_pointer", rts);
		
		//zawijamy wpis gdy jest zbyt długi 
		label = GTK_LABEL(gtk_bin_get_child(GTK_BIN(clip_content_menu)));
		gtk_label_set_single_line_mode(label, TRUE);
		gtk_label_set_ellipsize(label, PANGO_ELLIPSIZE_MIDDLE);
		gtk_label_set_max_width_chars(label, menu_width);
		
		g_signal_connect(clip_content_menu, "activate", G_CALLBACK (on_menuitem_clicked), contextMenu);
		
		if (clip->active == TRUE) {
			GtkWidget *image = gtk_image_new_from_stock("gtk-apply", GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(clip_content_menu), image);
		}
		
		gtk_container_add (GTK_CONTAINER (contextMenu), clip_content_menu);
		gtk_widget_show (clip_content_menu);
	}
}

GtkWidget* glipperus_contextMenu_new (RuntimeSettings *rts) {
	g_print("W menu szerokosc = %d \n", runtime_settings_get_menu_width(rts));
  GtkWidget *contextMenu;
  GtkWidget *separator;
  GtkWidget *ustawienia;
  GtkWidget *info;
  GtkWidget *wyjscie;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  contextMenu = gtk_menu_new ();

  g_object_set_data(G_OBJECT(contextMenu), "RuntimeSettings_pointer", rts);
  g_list_foreach(glipperus_stored_items, (GFunc)glipperus_contextMenu_add_clips, contextMenu);  

  separator = gtk_menu_item_new ();
  gtk_widget_show (separator);
  gtk_container_add (GTK_CONTAINER (contextMenu), separator);
  gtk_widget_set_sensitive (separator, FALSE);

  ustawienia = gtk_image_menu_item_new_from_stock ("gtk-preferences", accel_group);
  gtk_widget_show (ustawienia);
  gtk_container_add (GTK_CONTAINER (contextMenu), ustawienia);
  g_signal_connect(ustawienia, "activate", G_CALLBACK(on_preferences_clicked), rts);

  info = gtk_image_menu_item_new_from_stock ("gtk-dialog-info", accel_group);
  gtk_widget_show (info);
  gtk_container_add (GTK_CONTAINER (contextMenu), info);
  g_signal_connect(info, "activate", G_CALLBACK(on_info_clicked), NULL);
  
  separator = gtk_menu_item_new ();
  gtk_widget_show (separator);
  gtk_container_add (GTK_CONTAINER (contextMenu), separator);
  gtk_widget_set_sensitive (separator, FALSE);

  wyjscie = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (wyjscie);
  gtk_container_add (GTK_CONTAINER (contextMenu), wyjscie);
  g_signal_connect(wyjscie, "activate", G_CALLBACK (on_exit_clicked), NULL);

  gtk_menu_set_accel_group (GTK_MENU (contextMenu), accel_group);

  return contextMenu;
}
