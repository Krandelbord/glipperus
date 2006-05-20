CC=gcc -Wall -g3
CFLAGS +=`pkg-config --cflags gtk+-2.0`
LDFLAGS +=`pkg-config --libs gtk+-2.0`

glipper: main.o docklet.o eggtrayicon.o menu.o stored_items.o key_grabber.o clipboard.o clip_item.o InfoWindow.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.c config.h docklet.h menu.h key_grabber.h stored_items.h clipboard.h
docklet.o: docklet.h docklet.c config.h menu.h eggtrayicon.h
menu.o: menu.h menu.c config.h clipboard.h stored_items.h InfoWindow.h
stored_items.o: stored_items.c stored_items.h config.h clip_item.h clipboard.h
key_grabber.o: key_grabber.h key_grabber.c config.h menu.h stored_items.h
clipboard.o: clipboard.h clipboard.c config.h stored_items.h
clip_item.o: clip_item.h clip_item.c stored_items.h
InfoWindow.o: InfoWindow.h InfoWindow.c config.h

po:
	$(MAKE) -C po/

TAGS: 
	ctags -R *

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	-rm *.o
	-rm glipper
	-rm tags
	$(MAKE) clean -C po/
