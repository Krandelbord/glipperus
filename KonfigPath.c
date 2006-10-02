#include <glib/gstdio.h>

#include "KonfigPath.h"

KonfigPath *konfig_path_new(gchar *program_name) {
	KonfigPath *konfig_path = g_new0(KonfigPath, 1);
	
	const gchar *main_config_dir = g_get_user_config_dir();
	
	if ( !g_file_test(main_config_dir, G_FILE_TEST_IS_DIR)) {
		/* If the main config directory doesn't existis */
		mkdir(main_config_dir, S_IRWXU);
	}
	
	gchar *config_dir = g_build_filename(main_config_dir, program_name, NULL);
	if ( !g_file_test(config_dir, G_FILE_TEST_IS_DIR)) {
		/* If the config directory doesn't existis */
		mkdir(config_dir, S_IRWXU);
	}
	
	konfig_path->configuration = g_build_filename(config_dir, "config.conf", NULL);
	konfig_path->last_entries  = g_build_filename(config_dir, "last_entriest.conf", NULL);
	
	g_free(config_dir);
	
	return konfig_path;
}

gboolean konfig_path_file_exists(KonfigPath *konf_path) {
	return g_file_test(konf_path->configuration, G_FILE_TEST_EXISTS);
}

void konfig_path_write_default_configuration(KonfigPath *konf_path) {
	GKeyFile *config_keyfile = g_key_file_new();
	
	g_key_file_set_boolean(config_keyfile, "main", "override primary", FALSE);
	
	g_key_file_set_boolean(config_keyfile, "main", "transparent tray", FALSE);
	
	g_key_file_set_integer(config_keyfile, "main", "tray icon size", 2);
	
	g_key_file_set_integer(config_keyfile, "main", "number of menuitems", 5);
	
	g_key_file_set_integer(config_keyfile, "main", "menu entry width", 40);
	
	gchar *raw_keyfile = g_key_file_to_data(config_keyfile, NULL, NULL);
	
	FILE *fp = fopen(konf_path->configuration, "w");
	fprintf(fp, raw_keyfile);
	fclose(fp);
	g_key_file_free(config_keyfile);
	g_free(raw_keyfile);
}

void konfig_path_free(KonfigPath *konf_path) {
	g_free(konf_path->configuration);
	g_free(konf_path->last_entries);
	
	g_free(konf_path);
}
