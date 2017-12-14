CFLAGS+=-Wall -g3
#CFLAGS+=-O2
prefix=/usr/

# internal stuff - usally you don't have to edit below this line
CPPFLAGS+=-DGETTEXT_PATH=\"$(prefix)/share/locale\"
CFLAGS +=`pkg-config --cflags gtk+-2.0 glib-2.0`
LDFLAGS +=`pkg-config --libs gtk+-2.0 glib-2.0` -lX11

glipperus: main.o docklet.o eggtrayicon.o menu.o stored_items.o key_grabber.o clipboard.o clip_item.o InfoWindow.o KonfigPath.o ConfigDialog.o ConfigWidget.o RuntimeSettings.o HelpWindow.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

main.o: main.c docklet.h RuntimeSettings.h key_grabber.h stored_items.h clip_item.h clipboard.h KonfigPath.h config.h
docklet.o: docklet.c docklet.h RuntimeSettings.h eggtrayicon.h menu.h config.h
menu.o: menu.c menu.h RuntimeSettings.h clipboard.h stored_items.h clip_item.h InfoWindow.h ConfigDialog.h config.h
stored_items.o: stored_items.c stored_items.h RuntimeSettings.h clip_item.h clipboard.h config.h
key_grabber.o: key_grabber.c key_grabber.h RuntimeSettings.h menu.h config.h stored_items.h clip_item.h clipboard.h
clipboard.o: clipboard.c stored_items.h RuntimeSettings.h clip_item.h clipboard.h config.h
clip_item.o: clip_item.c stored_items.h RuntimeSettings.h clip_item.h clipboard.h
InfoWindow.o: InfoWindow.c InfoWindow.h config.h
KonfigPath.o: KonfigPath.c KonfigPath.h
ConfigDialog.o: ConfigDialog.c ConfigDialog.h RuntimeSettings.h KonfigPath.h ConfigWidget.h HelpWindow.h config.h
ConfigWidget.o: ConfigWidget.c ConfigWidget.h RuntimeSettings.h
RuntimeSettings.o: RuntimeSettings.c RuntimeSettings.h config.h
HelpWindow.o: HelpWindow.c HelpWindow.h config.h

po:
	$(MAKE) -C po/

TAGS: 
	ctags -R *

%.o: %.c %.h
	$(CC) $< -c $(CFLAGS) -o $@

install:
	install -d $(DESTDIR)$(prefix)/bin
	install glipperus $(DESTDIR)$(prefix)/bin/
	$(MAKE) install prefix=$(prefix) -C po/

.PHONY : clean
clean :
	-rm *.o
	-rm glipperus
	-rm tags
	$(MAKE) clean -C po/
