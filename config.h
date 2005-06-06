#define CLIP_GRABBER_INTERVAL_MS 200
#define KEY_GRABBER_INTERVAL_MS 400

#define MAX_NR_OF_ENTRIES 5

#define MAX_TXT_WIDTH_IN_CAHRS 40

#define POPUP_MENU_KEY "v"
#define COPY_KEY "CTRL+INS"
#define INSERT_KEY "SHIFT+INSERT"

#ifdef DEBUG
#  define glipper_debug(args...) g_print (args)
#else
#  define glipper_debug(args...) ;
#endif

