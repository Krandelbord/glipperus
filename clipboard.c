#include <gtk/gtk.h>
#include "stored_items.h"
#include "config.h"

typedef struct {
	GtkClipboard *primaryClipboard;
	GtkClipboard *defaultClipboard;
} glipper_all_clip;


void glipper_clipboard_set_txt(const gchar *newtxt) {
	GtkClipboard *defCpb = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(defCpb, newtxt, -1);	
}

static gboolean glipper_clipchange_listener (gpointer dane) {
	glipper_all_clip *all_clips = dane;
	/*GtkClipboard *primaryClipboard = dane;
	GtkClipboard *defaultClipboard; */
	glipper_debug("Działanie w timeoucie zmiany schowka \n");
	gchar *primtxt = NULL;
	gchar *cliptxt = NULL;

	primtxt = gtk_clipboard_wait_for_text (all_clips->primaryClipboard);

	if (primtxt != NULL) {
		glipper_debug("Tekst primary, czyli od samego znaznaczenia jest(%p) = %s \n", primtxt, primtxt);
		glipper_stored_items_add(glipper_clip_item_new_with_txt(primtxt));
	}
	
	glipper_debug("Sprawdzenie zwykłego clipboardu...");
	cliptxt = gtk_clipboard_wait_for_text (all_clips->defaultClipboard);

	if (cliptxt != NULL) {
		glipper_debug("Default czyli Clipboard jest taki %s \n", cliptxt);
		glipper_stored_items_add(glipper_clip_item_new_with_active_txt(cliptxt));
		//gtk_clipboard_set_text(primaryClip, txt, -1);
	}
	g_free(primtxt);
	g_free(cliptxt);
	primtxt = NULL;
	cliptxt = NULL;
	return TRUE;
}

void glipper_clip_grabber(void) {
	//gint czas;
	glipper_all_clip *all_clips;
	all_clips = g_malloc(sizeof(glipper_all_clip));
	all_clips->defaultClipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	all_clips->primaryClipboard = gtk_clipboard_get (GDK_SELECTION_PRIMARY);
	
	all_clips->defaultClipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	all_clips->primaryClipboard = gtk_clipboard_get (GDK_SELECTION_PRIMARY);
	
	g_timeout_add(CLIP_GRABBER_INTERVAL_MS, (GSourceFunc)glipper_clipchange_listener, all_clips);
}