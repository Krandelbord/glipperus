#include <gtk/gtk.h>

#include "docklet.h"
#include "menu.h"
#include "key_grabber.h"
#include "stored_items.h"
#include "clipboard.h"

#ifndef RUNTIME_SETTINGS_H
	#include "RuntimeSettings.h"
#endif

#include "KonfigPath.h"
#include "config.h"

#include <libintl.h>
#include <locale.h>


#include "ConfigDialog.h"


extern GList *glipper_stored_items; 

int main(int argc, char *argv[]) {
	//setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, "/home/emil/programing/C/gtk/glipper/trunk/");
	bind_textdomain_codeset(PACKAGE, "UTF-8");
	textdomain(PACKAGE);

	

	gtk_init(&argc, &argv);
	
	KonfigPath *kfpath = konfig_path_new("glipper");
	RuntimeSettings *rts = runtime_settings_new(kfpath->configuration);
	konfig_path_free(kfpath);
	
	
	GtkWidget *okno = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	glipper_stored_items_new();
	docklet_create(rts);

	glipper_clip_grabber();
	
	//glipper_assign_keygrab();
	
	//config_dialog_new();
	
	gtk_main();

	return 0;
}
