/* docklet.h
 * Contributed by Line72 <line72@line72.homelinux.com>
 */

#include <gtk/gtk.h>
#include "eggtrayicon.h"


GtkTooltips *tooltips;
GtkWidget *box;

gboolean docklet_create(GtkWidget *win);

void docklet_toggle();

void docklet_delete();
