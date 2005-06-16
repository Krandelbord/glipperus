#include <gtk/gtk.h>

#include "docklet.h"
#include "menu.h"
#include "key_grabber.h"
#include "stored_items.h"
#include "clipboard.h"
#include "config.h"

extern GList *glipper_stored_items; 

int main(int argc, char *argv[]) {

	GtkWidget *okno;

	gtk_init(&argc, &argv);
	
	okno = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	glipper_stored_items_new();
	docklet_create(okno);

	glipper_clip_grabber();
	
	glipper_assign_keygrab();
	
	gtk_main();


	return 0;
}
