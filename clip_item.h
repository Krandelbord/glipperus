#include <gtk/gtk.h>

typedef struct {
	gchar *contents;
	gboolean active;
} glipper_clip_item;

glipper_clip_item *glipper_clip_item_new_with_txt(gchar *new_txt);
glipper_clip_item *glipper_clip_item_new_with_active_txt(gchar *new_txt);
void glipper_clip_item_destroy(glipper_clip_item *clip_item_to_destroy);
