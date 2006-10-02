/* docklet.c
 * Contributed by Line72 <line72@line72.homelinux.com>
 */
#include "docklet.h"
#include "menu.h"
#include "config.h"

static void docklet_clicked(GtkWidget *button, GdkEventButton *event, gpointer *data);
static void docklet_destroyed(GtkWidget *widget, void *data);

static EggTrayIcon *docklet = NULL;

GtkWidget *image;


gboolean docklet_create(RuntimeSettings *rts) {
	docklet = egg_tray_icon_new("glipperus");
	box = gtk_event_box_new();
	image = gtk_image_new();

	g_signal_connect(G_OBJECT(docklet), "destroy", G_CALLBACK(docklet_destroyed), NULL);
	g_signal_connect(G_OBJECT(box), "button-press-event", G_CALLBACK(docklet_clicked), rts);

	//gtk_image_set_from_stock(GTK_IMAGE(image), GTK_STOCK_PASTE, GTK_ICON_SIZE_LARGE_TOOLBAR);
	
	
	/* we save evereythin from 0, but I wan't this number be compatiblile
	 * with GtkIconSize so I add +1 to it
	 */
	int icon_size = runtime_settings_get_tray_icon_size(rts) + 1;  
	
	if (runtime_settings_get_trasparent_tray(rts)) {
		GdkPixbuf *pbxf = gtk_widget_render_icon(GTK_WIDGET(docklet), GTK_STOCK_PASTE, icon_size, NULL);
		
		GdkBitmap *maska;
		gdk_pixbuf_render_pixmap_and_mask(pbxf, NULL, &maska, 128);
		gtk_widget_shape_combine_mask(GTK_WIDGET(docklet), maska, 0, 0);
		gtk_image_set_from_pixbuf(GTK_IMAGE(image), pbxf);
		g_object_unref(pbxf);
	} else {
		gtk_image_set_from_stock(GTK_IMAGE(image), GTK_STOCK_PASTE, icon_size);
	}
	gtk_widget_show(image);

	gtk_container_add(GTK_CONTAINER(box), image);

	gtk_container_add(GTK_CONTAINER(docklet), box);
	gtk_widget_show_all(GTK_WIDGET(docklet));

	g_object_ref(G_OBJECT(docklet));

	GtkTooltips *tooltipy = gtk_tooltips_new ();
	gtk_tooltips_set_tip (tooltipy, GTK_WIDGET( docklet), _("glipperus - thy powered clipboard tool"), NULL);

	return FALSE;
}

void docklet_clicked(GtkWidget *button, GdkEventButton *event, gpointer *data) {
	RuntimeSettings *rts = (RuntimeSettings*) data;
	GtkWidget *menu;

	if (event->type != GDK_BUTTON_PRESS)
		return;

	if (event->button == 3 || event->button == 1 || event->button == 2)
	{
		menu = glipperus_contextMenu_new(rts);
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
