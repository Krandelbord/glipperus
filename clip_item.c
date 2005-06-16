#include <gtk/gtk.h>
#include "stored_items.h"

glipper_clip_item *glipper_clip_item_new_with_txt(gchar *new_txt) {
	glipper_clip_item *new_gcp;
	new_gcp = (glipper_clip_item *) g_malloc(sizeof(glipper_clip_item));
	new_gcp->contents = g_strdup(new_txt);
	new_gcp->active = FALSE;
	return new_gcp;
}

glipper_clip_item *glipper_clip_item_new_with_active_txt(gchar *new_txt) {
	glipper_clip_item *new_gcp;
	
	glipper_stored_items_set_active(new_txt);
	new_gcp = glipper_clip_item_new_with_txt(new_txt);
	new_gcp->active = TRUE;
	return (new_gcp);
}

void glipper_clip_item_destroy(glipper_clip_item *clip_item_to_destroy) {
	g_free( clip_item_to_destroy->contents );
	clip_item_to_destroy->contents = NULL;
	g_free(clip_item_to_destroy);
}
