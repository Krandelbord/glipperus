#include "menu.h"
#include "clipboard.h"
#include "stored_items.h"
#include "config.h"

extern GList *glipper_stored_items;

static void on_menuitem_clicked (GtkImageMenuItem *menuitem, gpointer oneContextMenu) {
	GtkWidget *label = gtk_bin_get_child(GTK_BIN(menuitem));
	const gchar *txt = gtk_label_get_text(GTK_LABEL(label));

	glipper_stored_items_set_active(txt);
	g_print("Wybrałeś wpis = %s \n ",  txt);
	glipper_clipboard_set_txt(txt);
	gtk_widget_destroy(GTK_WIDGET(oneContextMenu));
}


static void on_exit_clicked (GtkMenuItem *menuitem, gpointer user_data) {
	g_print("Asta la pasta... bejbe\n");
	gtk_main_quit();
}

static void glipper_contextMenu_add_clips (gpointer data, gpointer user_data) {
	GtkWidget *contextMenu = user_data;
	glipper_clip_item *clip = data;
	GtkWidget *clip_content_menu;
	GtkLabel *label;

	if (clip != NULL) {
		clip_content_menu = gtk_image_menu_item_new_with_label (clip->contents);
		//zawijamy wpis gdy jest zbyt długi 
		label = GTK_LABEL(gtk_bin_get_child(GTK_BIN(clip_content_menu)));
		gtk_label_set_single_line_mode(label, TRUE);
		gtk_label_set_ellipsize(label, PANGO_ELLIPSIZE_MIDDLE);
		gtk_label_set_max_width_chars(label, MAX_TXT_WIDTH_IN_CAHRS);
		
		g_signal_connect(clip_content_menu, "activate", G_CALLBACK (on_menuitem_clicked), contextMenu);
		
		if (clip->active == TRUE) {
			GtkWidget *image = gtk_image_new_from_stock("gtk-apply", GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(clip_content_menu), image);
		}
		
		gtk_container_add (GTK_CONTAINER (contextMenu), clip_content_menu);
		gtk_widget_show (clip_content_menu);
	}
}

GtkWidget* glipper_contextMenu_new (void) {
  GtkWidget *contextMenu;
  GtkWidget *separator;
  GtkWidget *ustawienia;
  GtkWidget *info;
  GtkWidget *wyjscie;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  contextMenu = gtk_menu_new ();

  g_list_foreach(glipper_stored_items, (GFunc)glipper_contextMenu_add_clips, contextMenu);  

  separator = gtk_menu_item_new ();
  gtk_widget_show (separator);
  gtk_container_add (GTK_CONTAINER (contextMenu), separator);
  gtk_widget_set_sensitive (separator, FALSE);

  ustawienia = gtk_image_menu_item_new_from_stock ("gtk-preferences", accel_group);
  gtk_widget_show (ustawienia);
  gtk_container_add (GTK_CONTAINER (contextMenu), ustawienia);

  info = gtk_image_menu_item_new_from_stock ("gtk-dialog-info", accel_group);
  gtk_widget_show (info);
  gtk_container_add (GTK_CONTAINER (contextMenu), info);

  info = gtk_image_menu_item_new_from_stock("gtk-refresh", accel_group);
  gtk_widget_show (info);
  g_signal_connect(info, "activate", G_CALLBACK(glipper_clip_grabber), NULL);
  gtk_container_add (GTK_CONTAINER (contextMenu), info);
  
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
