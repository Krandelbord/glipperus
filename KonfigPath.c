#include <glib/gstdio.h>

#include "KonfigPath.h"

KonfigPath *konfig_path_new(gchar *program_name) {
	KonfigPath *konfig_path = g_new0(KonfigPath, 1);
	
	const gchar *main_config_dir = g_get_user_config_dir();
	
	if ( !g_file_test(main_config_dir, G_FILE_TEST_IS_DIR)) {
		/* If the main config directory doesn't existis */
		mkdir(main_config_dir, 1);
	}
	
	gchar *config_dir = g_build_filename(main_config_dir, program_name, NULL);
	if ( !g_file_test(config_dir, G_FILE_TEST_IS_DIR)) {
		/* If the config directory doesn't existis */
		mkdir(config_dir, 1);
	}
	
	konfig_path->configuration = g_build_filename(config_dir, "config.conf", NULL);
	konfig_path->last_entries  = g_build_filename(config_dir, "last_entriest.conf", NULL);
	
	g_free(config_dir);
	
	return konfig_path;
}

void konfig_path_free(KonfigPath *konf_path) {
	g_free(konf_path->configuration);
	g_free(konf_path->last_entries);
	
	g_free(konf_path);
}
