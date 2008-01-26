include Metadata

LDFLAGS = $(shell pkg-config --libs gtk+-2.0)
CFLAGS += -Wall
CFLAGS += $(RPM_OPT_FLAGS)
CFLAGS += $(shell pkg-config --cflags gtk+-2.0) -DGTK_DISABLE_BROKEN -DGTK_DISABLE_DEPRECATED
CFLAGS += -DPROJNAME='"$(PROJNAME)"' -DVERSION='"$(VERSION)"'

SRC = util stock theme_sel font_sel preview_pane about_dialog mainwin main

EXTRAS = $(addprefix $(EXENAME), .spec .spec.in .pod .pod.in .1) \
		 stock_menu_about.xpm Makefile Metadata replace COPYING ChangeLog

all: $(EXENAME) $(EXENAME).1

$(EXENAME): $(addsuffix .o, $(SRC))
	$(CC) $(LDFLAGS) $^ -o $@

$(EXENAME).1: $(EXENAME).pod
	pod2man $< | sed 's/^\.TH .*/.TH $(EXENAME) 1/' > $@

$(EXENAME).pod: $(EXENAME).pod.in Metadata
	./replace Metadata $< > $@ || rm $@

$(EXENAME).spec: $(EXENAME).spec.in Metadata
	./replace Metadata $< > $@ || rm $@

depend: $(addsuffix .c, $(SRC)) $(addsuffix .h, $(SRC))
	makedepend -Y $^ 2> /dev/null

clean:
	-rm -f $(EXENAME) $(addsuffix .o, $(SRC))

distclean: clean depend
	-rm -f $(EXENAME).spec $(EXENAME).1 Makefile.bak

install: all
	strip      $(EXENAME)
	install -d              $(DESTDIR)/$(PREFIX)/bin
	install -c $(EXENAME)   $(DESTDIR)/$(PREFIX)/bin
	install -d              $(DESTDIR)/$(PREFIX)/man/man1
	install -c $(EXENAME).1 $(DESTDIR)/$(PREFIX)/man/man1

DISTFILES = $(addsuffix .c, $(SRC)) $(addsuffix .h, $(SRC)) $(EXTRAS)
DISTNAME = $(EXENAME)-$(VERSION)
dist: $(DISTFILES)
	rm -rf /tmp/$(DISTNAME)
	mkdir /tmp/$(DISTNAME)
	cp -a $(DISTFILES) /tmp/$(DISTNAME)/
	tar cvjf ../$(DISTNAME).tar.bz2 -C /tmp $(DISTNAME)/
	rm -rf /tmp/$(DISTNAME)

.PHONY: clean all install dist depend

# DO NOT DELETE

util.o: util.h
stock.o: stock.h stock_menu_about.xpm
theme_sel.o: theme_sel.h main.h
font_sel.o: font_sel.h main.h
preview_pane.o: preview_pane.h util.h stock.h
about_dialog.o: about_dialog.h util.h stock.h
mainwin.o: main.h stock.h theme_sel.h font_sel.h preview_pane.h
mainwin.o: about_dialog.h
main.o: main.h stock.h mainwin.h
