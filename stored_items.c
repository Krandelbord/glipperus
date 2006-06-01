#include <gtk/gtk.h>
#include "stored_items.h"
#include "config.h"

GList *glipper_stored_items = NULL;
//extern GList *glipper_stored_items;
/**
 * It just initializes ptr array for initial test :-P
 * @returns newly allocated glist
 **/
void glipper_stored_items_new(void) {
	glipper_stored_items_add (glipper_clip_item_new_with_txt(_("first default text")));
	glipper_stored_items_add (glipper_clip_item_new_with_txt(_("second default text..")));
	glipper_stored_items_add (glipper_clip_item_new_with_txt(_("third default text")));
	glipper_stored_items_add (glipper_clip_item_new_with_active_txt(_("active default text")) );
}

gint szukaj_wpisu (gconstpointer a, gconstpointer b) {
	const glipper_clip_item *first = a;
	const glipper_clip_item *second = b;
	return g_ascii_strcasecmp (first->contents, second->contents);
}

static void stored_items_change_active (gpointer data, gpointer user_data) {
	glipper_clip_item *clip = data;
	const gchar *new_active = user_data;
	if ( (g_ascii_strcasecmp(clip->contents, new_active)) == 0) {
		clip->active = TRUE;
	} else {
		clip->active = FALSE;
	}
}

void glipper_stored_items_set_active(const gchar *new_active) {
	g_list_foreach(glipper_stored_items, (GFunc)stored_items_change_active, (gchar *)new_active);
}

void glipper_stored_items_add(glipper_clip_item *new_item) {
	GList *znaleziony;
	glipper_debug("Zaczynamy dopisywanie do listy elementu %s\n", new_item->contents);
	
	if (g_list_find_custom(glipper_stored_items, new_item, (GCompareFunc)szukaj_wpisu) ) {
		/*
		 * jeżeli wpis już jest na liście to nic nie robimy...
		 * no może poza zwolnieniem pamięci 
		 */
		znaleziony = g_list_find_custom(glipper_stored_items, new_item, (GCompareFunc)szukaj_wpisu);
		glipper_debug("Kurde znaleziono już w liście %s \n", ((glipper_clip_item *)znaleziony->data)->contents );
		glipper_clip_item_destroy(new_item);
		return;
	}
	guint ilosc = 0;
	ilosc = g_list_length(glipper_stored_items);
	while (ilosc > MAX_NR_OF_ENTRIES) {
		glipper_clip_item *nadmiar= g_list_nth_data(glipper_stored_items, ilosc-1);
		glipper_debug("powinno być max %d wpisów, a jest %d\n", MAX_NR_OF_ENTRIES, ilosc);
		glipper_debug("Usuwanie elementu %d (%p) = %s \n", ilosc-1, nadmiar, nadmiar->contents);
		//najsampierw zwalniamy pamięć gdzie rezydują dane z wpisem
		glipper_clip_item_destroy(nadmiar);	
		glipper_stored_items = g_list_delete_link(glipper_stored_items, g_list_nth(glipper_stored_items, ilosc-1)); //usuwamy link (wskazuje już na puste miejsce
		ilosc = g_list_length(glipper_stored_items);
	}
	glipper_stored_items = g_list_prepend (glipper_stored_items, (gpointer) new_item);
}
