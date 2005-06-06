#include <gtk/gtk.h>
#include "clip_item.h"
#include "clipboard.h"

void glipper_stored_items_new(void);
void glipper_stored_items_add(glipper_clip_item *new_item);
void glipper_stored_items_set_active(const gchar *new_active);
