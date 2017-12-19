#include "docklet.h"
#include "menu.h"
#include "config.h"

static void docklet_clicked(GtkWidget *button, GdkEventButton *event, gpointer *data);

static GtkStatusIcon *docklet = NULL;

GtkWidget *image;

gboolean docklet_create(RuntimeSettings *rts) {
	docklet = gtk_status_icon_new_from_stock(GTK_STOCK_PASTE);
	g_signal_connect(docklet, "button-press-event", G_CALLBACK(docklet_clicked), rts);
    gtk_status_icon_set_tooltip_markup(docklet, _("glipperus - thy powered clipboard tool"));
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

