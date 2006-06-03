#include <gtk/gtk.h>
#include "stored_items.h"
#include "config.h"

GList *glipper_stored_items = NULL;
//extern GList *glipper_stored_items;
/**
 * It just initializes ptr array for initial test :-P
 * @returns newly allocated glist
 **/
void glipper_stored_items_new(RuntimeSettings *rts) {
	glipper_stored_items_add (glipper_clip_item_new_with_txt(_("first default text")), rts);
	glipper_stored_items_add (glipper_clip_item_new_with_txt(_("second default text..")), rts);
	glipper_stored_items_add (glipper_clip_item_new_with_txt(_("third default text")), rts);
	glipper_stored_items_add (glipper_clip_item_new_with_active_txt(_("active default text")), rts);
}

gint szukaj_wpisu (gconstpointer a, gconstpointer b) {
	const glipper_clip_item *first = a;
	const glipper_clip_item *second = b;
	return g_ascii_strcasecmp (first->contents, second->contents);
}

gint search_starting_with(gconstpointer a, gconstpointer b) {
	const glipper_clip_item *first = a;
	const glipper_clip_item *second = b;
	/* Returns : 	 TRUE if str begins with prefix, FALSE otherwise. */
	gboolean wynik_por = g_str_has_prefix (second->contents, first->contents);
	
	if (wynik_por) {
		/* When exists item contains part of the new item */
		return 0;
	} else return -1;
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

void glipper_stored_items_add(glipper_clip_item *new_item, RuntimeSettings *rts) {
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
	
	gboolean remove_similar = runtime_settings_get_overwrite_similar(rts);
	if (remove_similar) {
		/* Check if we have a entry which start like our new entry */
		GList *found_similar_entry = g_list_find_custom(glipper_stored_items, new_item, (GCompareFunc)search_starting_with);
		if (found_similar_entry!=NULL) {
			glipper_debug("Found entry starting with the same text \n");
			gint pos_of_found_item = g_list_position(glipper_stored_items, found_similar_entry);
			glipper_clip_item *to_remove = g_list_nth_data(glipper_stored_items, pos_of_found_item);
			glipper_clip_item_destroy(to_remove);
			glipper_stored_items = g_list_delete_link(glipper_stored_items, found_similar_entry);
		}
	}
	
	gint max_nr_of_entries = runtime_settings_get_number_of_entries(rts);
	while (ilosc > max_nr_of_entries) { /* If the list is too long (specyfied by usr options */
		/* Get the last entry */
		glipper_clip_item *nadmiar= g_list_nth_data(glipper_stored_items, ilosc-1);
		glipper_debug("There is a limt = %d entires, but now we have %d\n", max_nr_of_entries, ilosc);
		glipper_debug("Removing entry %d (%p) = %s \n", ilosc-1, nadmiar, nadmiar->contents);
		
		//Fist we free the memory where the entry is stored
		glipper_clip_item_destroy(nadmiar);
		
		//removing lik which points to empty space
		glipper_stored_items = g_list_delete_link(glipper_stored_items, g_list_nth(glipper_stored_items, ilosc-1));
	}
	glipper_stored_items = g_list_prepend (glipper_stored_items, (gpointer) new_item);
}
