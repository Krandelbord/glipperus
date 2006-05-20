CC=gcc -Wall -g3
CFLAGS +=`pkg-config --cflags gtk+-2.0`
LDFLAGS +=`pkg-config --libs gtk+-2.0`

glipper: main.o docklet.o eggtrayicon.o menu.o stored_items.o key_grabber.o clipboard.o clip_item.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
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
