/* docklet.h
 * Contributed by Line72 <line72@line72.homelinux.com>
 */

#include <gtk/gtk.h>
#include "eggtrayicon.h"

#ifndef RUNTIME_SETTINGS_H
	#include "RuntimeSettings.h"
#endif

GtkTooltips *tooltips;
GtkWidget *box;

gboolean docklet_create(RuntimeSettings *rts);

void docklet_toggle();

void docklet_delete();
