translations := $(wildcard *.po)
mo_files:= $(patsubst %.po,%.mo,$(translations))

all: glipperus.pot $(mo_files)

glipperus.pot:
	xgettext -cTranslators: --keyword=_ ../*.c ../*.h -o $@

#updates all translation based on current pot file
$(translations): glipperus.pot
	msgmerge $@ $< -o $@

#generates all *.mo files
$(mo_files): $(translations)
	msgfmt $< -o $@


update-po: $(translations)

install:
	msgfmt pl.po -o ../pl/LC_MESSAGES/glipperus.mo

.PHONY : clean
clean :
	-rm glipperus.pot
	-rm $(mo_files)
