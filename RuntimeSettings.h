#define RUNTIME_SETTINGS_H

#include <gtk/gtk.h>

typedef struct _KeybModifiers {
	gboolean ctrl;
	gboolean alt;
	gboolean shift;
} KeybModifiers;

typedef struct _RuntimeSettings {
	gboolean override_selection;
	gboolean overwrite_similar;
	gboolean trasparent_tray;
	
	int tray_icon_size;
	int number_of_entries;
	int menu_width;
	
	KeybModifiers *keyb_modifiers;
	int keycode;
} RuntimeSettings;


RuntimeSettings *runtime_settings_new(gchar *path_to_config_file);
void runtime_settings_read_from_file(RuntimeSettings *rts, GKeyFile *keyfile);

void runtime_settings_set_override_sel(RuntimeSettings *rts, gboolean override_selection);
void runtime_settings_set_overwrite_similar(RuntimeSettings *rts, gboolean overwrite_similar);
void runtime_settings_set_trasparent_tray(RuntimeSettings *rts, gboolean trasparent_tray);

gboolean runtime_settings_get_override_sel(RuntimeSettings *rts);
gboolean runtime_settings_get_overwrite_similar(RuntimeSettings *rts);
gboolean runtime_settings_get_trasparent_tray(RuntimeSettings *rts);

/* the init seetings */
void runtime_settings_set_tray_icon_size(RuntimeSettings *rts, int tray_icon_size);
void runtime_settings_set_number_of_entries(RuntimeSettings *rts, int numb_entr);
void runtime_settings_set_menu_width(RuntimeSettings *rts, int);

int  runtime_settings_get_tray_icon_size(RuntimeSettings *rts);
int  runtime_settings_get_number_of_entries(RuntimeSettings *rts);
int  runtime_settings_get_menu_width(RuntimeSettings *rts);
