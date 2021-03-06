#ifndef CONFIG_H
#define CONFIG_H

#define CLIP_GRABBER_INTERVAL_MS 200
#define KEY_GRABBER_INTERVAL_MS 400

#define MAX_NR_OF_ENTRIES 15

#define MAX_TXT_WIDTH_IN_CAHRS 40

#define POPUP_MENU_KEY "v"
#define COPY_KEY "CTRL+INS"
#define INSERT_KEY "SHIFT+INSERT"

#include <libintl.h>
#define PACKAGE "glipperus"
#define _(s) gettext(s)

#ifdef DEBUG
#  define glipperus_debug(args...) g_print (args)
#else
#  define glipperus_debug(args...) ;
#endif


#endif /* CONFIG_H */
