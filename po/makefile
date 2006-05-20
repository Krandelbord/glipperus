translations := $(wildcard *.po)
mo_files:= $(patsubst %.po,%.mo,$(translations))

all: glipper.pot $(mo_files)

glipper.pot:
	xgettext --keyword=_ ../*.c ../*.h -o $@

#updates all translation based on current pot file
$(translations): glipper.pot
	msgmerge $@ $< -o $@

#generates all *.mo files
$(mo_files): $(translations)
	msgfmt $< -o $@


update-po: $(translations)

install:
	msgfmt pl.po -o ../pl/LC_MESSAGES/glipper.mo

.PHONY : clean
clean :
	-rm glipper.pot
	-rm $(mo_files)