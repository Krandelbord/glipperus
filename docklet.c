/* docklet.c
 * Contributed by Line72 <line72@line72.homelinux.com>
 */
#include "docklet.h"
#include "menu.h"

static void docklet_clicked(GtkWidget *button, GdkEventButton *event, void *data);
static void docklet_destroyed(GtkWidget *widget, void *data);

static EggTrayIcon *docklet = NULL;
static GtkWidget *main_window = NULL;

GtkWidget *image;


gboolean docklet_create(GtkWidget *win) {
  main_window = win;

  docklet = egg_tray_icon_new("glipper");
  box = gtk_event_box_new();
  image = gtk_image_new();

  g_signal_connect(G_OBJECT(docklet), "destroy", G_CALLBACK(docklet_destroyed), NULL);
  g_signal_connect(G_OBJECT(box), "button-press-event", G_CALLBACK(docklet_clicked), NULL);

  gtk_image_set_from_stock(GTK_IMAGE(image), GTK_STOCK_PASTE, GTK_ICON_SIZE_LARGE_TOOLBAR);
  
  gtk_container_add(GTK_CONTAINER(box), image);

  gtk_container_add(GTK_CONTAINER(docklet), box);
  gtk_widget_show_all(GTK_WIDGET(docklet));

  g_object_ref(G_OBJECT(docklet));


  return FALSE;
}

void docklet_clicked(GtkWidget *button, GdkEventButton *event, void *data) {
	GtkWidget *menu;
	
  if (event->type != GDK_BUTTON_PRESS)
    return;

  if (event->button == 3 || event->button == 1 || event->button == 2)
	{
		menu = glipper_contextMenu_new();
	    gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
						  event->button, event->time);
	}
}

static void docklet_destroyed(GtkWidget *widget, void *data) {
  g_signal_handlers_disconnect_by_func(G_OBJECT(docklet), G_CALLBACK(docklet_destroyed), NULL);
  gtk_widget_destroy(GTK_WIDGET(docklet));
  g_object_unref(G_OBJECT(docklet));
  docklet = NULL;
}
