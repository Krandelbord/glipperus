#ifndef STORED_ITEMS_H
#define STORED_ITEMS_H

#include <gtk/gtk.h>
#include "RuntimeSettings.h"
#include "clip_item.h"
#include "clipboard.h"

void glipperus_stored_items_new(RuntimeSettings *rts);
void glipperus_stored_items_add(glipperus_clip_item *new_item, RuntimeSettings *rts);
void glipperus_stored_items_set_active(const gchar *new_active);

#endif
