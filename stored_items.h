#include <gtk/gtk.h>

#ifndef RUNTIME_SETTINGS_H
	#include "RuntimeSettings.h"
#endif

#include "clip_item.h"
#include "clipboard.h"

void glipperus_stored_items_new(RuntimeSettings *rts);
void glipperus_stored_items_add(glipperus_clip_item *new_item, RuntimeSettings *rts);
void glipperus_stored_items_set_active(const gchar *new_active);
