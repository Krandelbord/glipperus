#include <gtk/gtk.h>

#include "docklet.h"
#include "menu.h"
#include "key_grabber.h"
#include "stored_items.h"
#include "clipboard.h"
#include "config.h"

#include <libintl.h>
#include <locale.h>

#include <ctype.h>

extern GList *glipper_stored_items; 

int main(int argc, char *argv[]) {
	//setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, "/home/emil/programing/C/gtk/glipper/trunk/");
	bind_textdomain_codeset(PACKAGE, "UTF-8");
	textdomain(PACKAGE);

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
