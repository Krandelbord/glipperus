#ifndef DOCKLET_H
#define DOCKLET_H

#include <gtk/gtk.h>
#include "eggtrayicon.h"
#include "RuntimeSettings.h"


GtkTooltips *tooltips;
GtkWidget *box;

gboolean docklet_create(RuntimeSettings *rts);

void docklet_toggle();

void docklet_delete();

#endif
