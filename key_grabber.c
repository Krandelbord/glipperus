#include <gtk/gtk.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "key_grabber.h"
#include "menu.h"
#include "config.h"
#include "stored_items.h"

typedef enum {
	glipperus_ACTION_SHOW_MENU,
	glipperus_ACTION_COPY,
	glipperus_ACTION_PASTE,
} KeyAction;

typedef struct {
	guchar keycode, modifier;
	KeyAction action;
} Key;

GSList *kList = NULL; //List of keys

Display *dpy;
Window root;

int NumLockMask, CapsLockMask, ScrollLockMask;

void init_keyboard()
{
  XModifierKeymap *xmk=NULL;
  KeyCode *map;
  int m, k;
  
  xmk=XGetModifierMapping(dpy);
  if(xmk)
  {
    map=xmk->modifiermap;
    for(m=0;m<8;m++)
      for(k=0;k<xmk->max_keypermod; k++, map++)
      {
        if(*map==XKeysymToKeycode(dpy, XK_Num_Lock))
          NumLockMask=(1<<m);
        if(*map==XKeysymToKeycode(dpy, XK_Caps_Lock))
          CapsLockMask=(1<<m);
        if(*map==XKeysymToKeycode(dpy, XK_Scroll_Lock))
          ScrollLockMask=(1<<m);
	  }
    XFreeModifiermap(xmk);
  }
}

void grab_key(guchar keycode, unsigned int modifiers, Window w, KeyAction assigned_action) {
	Key *k;
	k = g_malloc(sizeof(k));
	k->keycode = keycode;

	k->modifier = modifiers;
	k->action = assigned_action;
	kList = g_slist_append(kList, k);
	
	glipperus_debug("Zarejestrowano do grabowania klawisz %d, z modifikatorem %d\n", k->keycode, k->modifier);
	//rejestrujemy grabowniae klawiszy dl różnyc stanów NumLocka Srocka itp...
	XGrabKey(dpy, keycode, modifiers, w, False, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, keycode, modifiers|NumLockMask, w, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, keycode, modifiers|CapsLockMask, w, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, keycode, modifiers|ScrollLockMask, w, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, keycode, modifiers|NumLockMask|CapsLockMask, w, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, keycode, modifiers|NumLockMask|ScrollLockMask, w, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(dpy, keycode, modifiers|NumLockMask|CapsLockMask|ScrollLockMask, w, True, GrabModeAsync, GrabModeAsync);
}

void initialize(void) {
	//opens default display
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		g_error(_("glipperus error: Cannot open default XDisplay.\n"));
	}
	//gets rootWindow
	root = XDefaultRootWindow(dpy);
	if (!root) {
		g_error(_("glipperus error: Cannot open root window\n"));
	}
	init_keyboard();
}

void position_thy_menu (GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer user_data) {
	XEvent *ev = user_data;
	glipperus_debug("%s jest menu\n", GTK_IS_MENU(menu) ?"Tak to":"Nie to nie");
	XNextEvent(dpy, ev); //zdejmujemy ten event
	push_in = (gboolean *) TRUE;
	x = (gint *) 100;
	y = (gint *) 100;
}

static void do_key_acton(KeyAction action, XEvent *ev) {
	glipperus_debug("Wykonujemy %d \n", action);
	switch (action) {
		case glipperus_ACTION_SHOW_MENU: {
			GtkWidget *menu;
			//FIXME: temportay null pointer 
			RuntimeSettings *rts;
			menu = glipperus_contextMenu_new(rts);
			gtk_menu_popup(GTK_MENU(menu),
					NULL, 
					NULL, (GtkMenuPositionFunc)position_thy_menu,
					ev, 
					ev->xbutton.button,
					ev->xbutton.time);
				glipperus_debug("Żygaj menu \n");
				break;
		}
		
		case glipperus_ACTION_PASTE: 
			glipperus_debug("Wklejasz ziom \n");
			XNextEvent(dpy, ev); //zdejmujemy ten event
			break;
		case glipperus_ACTION_COPY: 
			glipperus_debug("Kopiujuesz ziom \n");
			XNextEvent(dpy, ev); //zdejmujemy ten event
			break; 
		default:
		glipperus_debug("Printf próba wykonania nieznzny action nr. %d \n", action);
	}
}

void porownaj_klawisze(gpointer stored_Key, gpointer event) {
	Key *key = stored_Key;
	XEvent *ev = event; //hmm.. 
	ev->xkey.state=ev->xkey.state&(Mod1Mask|ControlMask|ShiftMask);
	if (key->keycode == ev->xkey.keycode && key->modifier==ev->xkey.state) do_key_acton(key->action, ev);
	
}

static void sprawdz_wcisk(XEvent *ev) {
	glipperus_debug("Porownanie klaiwszy\n");
	g_slist_foreach(kList, (GFunc)porownaj_klawisze, ev);
}

static gboolean nasluch_klawiszy(void *args) {
	glipperus_debug("Początek działania w timoucie \n");
		if(XPending(dpy)) {
			XEvent ev;
			//XPeekEvent(dpy, &ev);
			XNextEvent(dpy, &ev);
			if(ev.type==KeyPress) {
				XPutBackEvent(dpy, &ev);
				// czyli że prawie nie wiem o co chodzi: Do gtk_menu_popup musi 
				// być puszczony event aby mógł natysować to menu. Jeżeli byśmy go zdjęli 
				// i nie odłożyli (za pomocą XPutBackEvent) to manu się rysuje bardz-czasami. (Bez sensu).
				GtkWidget *menu;
				//FIXME: runtime kurde tymczose
				RuntimeSettings *rts;
				menu = glipperus_contextMenu_new(rts);
				//gtk_menu_set_screen (GTK_MENU(menu), gdk_screen_get_default());
				
				/*
				gdk_threads_enter();
				gtk_menu_popup(GTK_MENU(menu), 
						NULL, 
						NULL, (GtkMenuPositionFunc)position_thy_menu,
						&ev, 
						ev.xbutton.button,
						ev.xbutton.time);
				gdk_threads_leave();
				glipperus_debug("Klawiszologia = %d Powinno pojawić się menu\n", ev.xkey.keycode);
				*/
				sprawdz_wcisk(&ev);
			} //koniec przetwarzania zdarzenia KeyPressed
			glipperus_debug("Przetwarzamy zdzarznie typu %d \n", ev.type);
		}
		return TRUE;
}

void glipperus_assign_keygrab(RuntimeSettings *rts) {

	initialize();
	
	guchar modifier = 0;
	modifier = modifier|ControlMask;
	modifier = modifier|Mod1Mask;

	guchar keycode = XKeysymToKeycode(dpy, XStringToKeysym("v"));
	grab_key(keycode, modifier, root, glipperus_ACTION_SHOW_MENU);
	
	/* Ahh byłoby pięknie 
	modifier = 0;
	modifier = modifier|ControlMask;
	grab_key(106, modifier, root, glipperus_ACTION_COPY); */
	
	//g_thread_create(thread_nasluch_klawiszy, NULL, FALSE, &blad);
	g_timeout_add(KEY_GRABBER_INTERVAL_MS, (GSourceFunc)nasluch_klawiszy, NULL);
}
