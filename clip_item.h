#include <gtk/gtk.h>

typedef struct {
	gchar *contents;
	gboolean active;
} glipperus_clip_item;

glipperus_clip_item *glipperus_clip_item_new_with_txt(gchar *new_txt);
glipperus_clip_item *glipperus_clip_item_new_with_active_txt(gchar *new_txt);
void glipperus_clip_item_destroy(glipperus_clip_item *clip_item_to_destroy);
