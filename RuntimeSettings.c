#ifndef RUNTIME_SETTINGS_H
	#include "RuntimeSettings.h"
#endif

#include "config.h"

RuntimeSettings *runtime_settings_new(gchar *path_to_config) {
	RuntimeSettings *rts = g_new0(RuntimeSettings, 1);
	
	GKeyFile *kf = g_key_file_new();
	if ( g_key_file_load_from_file(kf, path_to_config, G_KEY_FILE_KEEP_COMMENTS, NULL) ) {
		
		rts->override_selection = g_key_file_get_boolean(kf, "main", "override primary", NULL);
		rts->overwrite_similar = g_key_file_get_boolean(kf, "main", "overwrite similar", NULL);
		rts->trasparent_tray = g_key_file_get_boolean(kf, "main", "transparent tray", NULL);
		
		rts->tray_icon_size = g_key_file_get_integer(kf, "main", "tray icon size", NULL);
		rts->number_of_entries = g_key_file_get_integer(kf, "main", "number of menuitems", NULL);
		rts->menu_width = g_key_file_get_integer(kf, "main", "menu entry width", NULL);
		
	} else {
		g_error(_("Error reading configuration file from: %s\n"), path_to_config);
	}
	g_key_file_free(kf);
	return rts;
}

void runtime_settings_set_override_sel(RuntimeSettings *rts, gboolean override_selection) {
	rts->override_selection = override_selection;
}

void runtime_settings_set_overwrite_similar(RuntimeSettings *rts, gboolean overwrite_similar) {
	rts->overwrite_similar = overwrite_similar;
}
void runtime_settings_set_trasparent_tray(RuntimeSettings *rts, gboolean trasparent_tray) {
	rts->trasparent_tray = trasparent_tray;
}

gboolean runtime_settings_get_override_sel(RuntimeSettings *rts) {
	return rts->override_selection;
}

gboolean runtime_settings_get_overwrite_similar(RuntimeSettings *rts) {
	return rts->overwrite_similar;
}

gboolean runtime_settings_get_trasparent_tray(RuntimeSettings *rts) {
	return rts->trasparent_tray;
}

/* the init seetings */
void runtime_settings_set_tray_icon_size(RuntimeSettings *rts, int tray_icon_size) {
	rts->tray_icon_size = tray_icon_size;
}

void runtime_settings_set_number_of_entries(RuntimeSettings *rts, int numb_entr) {
	rts->number_of_entries = numb_entr;
}

void runtime_settings_set_menu_width(RuntimeSettings *rts, int menu_width) {
	rts->menu_width = menu_width;
}


int  runtime_settings_get_tray_icon_size(RuntimeSettings *rts) {
	return rts->tray_icon_size;
}

int  runtime_settings_get_number_of_entries(RuntimeSettings *rts) {
	return rts->number_of_entries;
}

int  runtime_settings_get_menu_width(RuntimeSettings *rts) {
	return rts->menu_width;
}
