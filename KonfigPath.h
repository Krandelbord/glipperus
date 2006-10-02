#ifndef KONFIG_PATH_H
#define KONFIG_PATH_H

#include <gtk/gtk.h>

typedef struct _KonfigPath {
	gchar *configuration;
	gchar *last_entries;
} KonfigPath;

KonfigPath *konfig_path_new(gchar *program_name);

/* returns true if config file exist, else returns false */

gboolean konfig_path_file_exists(KonfigPath *konf_path);

void konfig_path_write_default_configuration(KonfigPath *konf_path);

void konfig_path_free(KonfigPath *konf_path);

#endif
