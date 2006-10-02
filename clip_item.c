#include <gtk/gtk.h>
#include "stored_items.h"

glipperus_clip_item *glipperus_clip_item_new_with_txt(gchar *new_txt) {
	glipperus_clip_item *new_gcp;
	new_gcp = (glipperus_clip_item *) g_malloc(sizeof(glipperus_clip_item));
	new_gcp->contents = g_strdup(new_txt);
	new_gcp->active = FALSE;
	return new_gcp;
}

glipperus_clip_item *glipperus_clip_item_new_with_active_txt(gchar *new_txt) {
	glipperus_clip_item *new_gcp;
	
	glipperus_stored_items_set_active(new_txt);
	new_gcp = glipperus_clip_item_new_with_txt(new_txt);
	new_gcp->active = TRUE;
	return (new_gcp);
}

void glipperus_clip_item_destroy(glipperus_clip_item *clip_item_to_destroy) {
	g_free(clip_item_to_destroy->contents);
	clip_item_to_destroy->contents = NULL;
	g_free(clip_item_to_destroy);
}
