translations := $(wildcard *.po)
mo_files:= $(patsubst %.po,%.mo,$(translations))
GETTEXT_DIR=../$(DESTDIR)$(prefix)share/locale/


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

TRANS_DIRS := $(patsubst %.po, $(GETTEXT_DIR)%/LC_MESSAGES/,$(translations))

create_dirs: *.po
	@echo "Creating directiories"
	@echo "Translations = $(TRANS_DIRS)"
	$(foreach, plik, $^, touch $plik)
	install -d $(TRANS_DIRS)

install-translations: *.po
	#for plik in $^ ; do echo $$plik ; done

install: create_dirs
	msgfmt pl.po -o $(GETTEXT_DIR)pl/LC_MESSAGES/glipperus.mo
	msgfmt uk.po -o $(GETTEXT_DIR)uk/LC_MESSAGES/glipperus.mo

.PHONY : clean
clean :
	-rm glipperus.pot
	-rm -f $(mo_files)
