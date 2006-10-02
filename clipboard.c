#include <gtk/gtk.h>
#include "stored_items.h"

#include "config.h"

typedef struct {
	GtkClipboard *primaryClipboard;
	GtkClipboard *defaultClipboard;
} glipperus_all_clip;


void glipperus_clipboard_set_txt(const gchar *newtxt) {
	gtk_clipboard_set_text(gtk_clipboard_get (GDK_SELECTION_CLIPBOARD), newtxt, -1);
}

static gboolean glipperus_clipchange_listener (gpointer dane) {
	RuntimeSettings *rts = (RuntimeSettings*) dane;
	/*GtkClipboard *primaryClipboard = dane;
	GtkClipboard *defaultClipboard; */
	glipperus_debug("Działanie w timeoucie zmiany schowka \n");
	gchar *primtxt = NULL;
	gchar *cliptxt = NULL;

	primtxt = gtk_clipboard_wait_for_text (gtk_clipboard_get(GDK_SELECTION_PRIMARY));

	if (primtxt != NULL) {
		glipperus_debug("Tekst primary, czyli od samego znaznaczenia jest(%p) = %s \n", primtxt, primtxt);
		glipperus_stored_items_add(glipperus_clip_item_new_with_txt(primtxt), rts);
	}
	
	glipperus_debug("Sprawdzenie zwykłego clipboardu...");
	cliptxt = gtk_clipboard_wait_for_text (gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));

	if (cliptxt != NULL) {
		glipperus_debug("Default czyli Clipboard jest taki %s \n", cliptxt);
		glipperus_stored_items_add(glipperus_clip_item_new_with_active_txt(cliptxt), rts);
		//gtk_clipboard_set_text(primaryClip, txt, -1);
	}
	g_free(primtxt);
	g_free(cliptxt);
	primtxt = NULL;
	cliptxt = NULL;
	return TRUE;
}

void glipperus_clip_grabber(RuntimeSettings *rts) {
	//gint czas;
	glipperus_all_clip *all_clips;
	all_clips = g_malloc(sizeof(glipperus_all_clip));
	all_clips->defaultClipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	all_clips->primaryClipboard = gtk_clipboard_get (GDK_SELECTION_PRIMARY);
	
	//all_clips->defaultClipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	//all_clips->primaryClipboard = gtk_clipboard_get (GDK_SELECTION_PRIMARY);
	
	g_timeout_add(CLIP_GRABBER_INTERVAL_MS, (GSourceFunc)glipperus_clipchange_listener, rts);
}
