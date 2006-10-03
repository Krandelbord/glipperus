#include <gtk/gtk.h>

#include "docklet.h"
#include "key_grabber.h"
#include "stored_items.h"
#include "clipboard.h"
#include "RuntimeSettings.h"
#include "KonfigPath.h"
#include "config.h"

#include <libintl.h>
#include <locale.h>

extern GList *glipperus_stored_items; 

int main(int argc, char *argv[]) {
	//setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, GETTEXT_PATH);
	bind_textdomain_codeset(PACKAGE, "UTF-8");
	textdomain(PACKAGE);

	

	gtk_init(&argc, &argv);
	
	KonfigPath *kfpath = konfig_path_new("glipperus");
	if (! konfig_path_file_exists(kfpath))
		konfig_path_write_default_configuration(kfpath);

	RuntimeSettings *rts = runtime_settings_new(kfpath->configuration);
	konfig_path_free(kfpath);
	
	
	//GtkWidget *okno = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	glipperus_stored_items_new(rts);
	docklet_create(rts);

	glipperus_clip_grabber(rts);

	glipperus_assign_keygrab(rts);
	
	gtk_main();

	return 0;
}
