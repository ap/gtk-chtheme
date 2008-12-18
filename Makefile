include Metadata

CFLAGS := $(if $(RPM_OPT_FLAGS), $(RPM_OPT_FLAGS), $(CFLAGS))

LDFLAGS += $(shell pkg-config --libs gtk+-2.0)
CFLAGS += -Wimplicit -Wall -std=gnu89 -pedantic-errors
CFLAGS += $(shell pkg-config --cflags gtk+-2.0) -DGTK_DISABLE_BROKEN -DGTK_DISABLE_DEPRECATED
CFLAGS += -DPROJNAME='"$(PROJNAME)"' -DVERSION='"$(VERSION)"'
CPPFLAGS =
CXXFLAGS =

SRC = util.c stock.c theme_sel.c font_sel.c preview_pane.c about_dialog.c mainwin.c main.c

EXTRAS = $(addprefix $(EXENAME), .spec .spec.in .pod .pod.in .1) \
		 stock_menu_about.png Makefile Metadata replace COPYING ChangeLog

.PHONY: all
all: $(EXENAME) $(EXENAME).1

%.png_c: %.png
	gdk-pixbuf-csource --raw --name=$* $< > $@

$(EXENAME): $(SRC:.c=.o)
	$(CC) $(LDFLAGS) $^ -o $@

%.1: %.pod
	pod2man $< | sed 's/^\.TH .*/.TH $(EXENAME) 1/' > $@

%.pod: %.pod.in Metadata
	./replace Metadata $< > $@ || rm $@

%.spec: %.spec.in Metadata
	./replace Metadata $< > $@ || rm $@

Makefile.depend: $(SRC)
	$(CC) -MM -MG -DUSE_OWN_ABOUT_STOCK $^ > $@

.PHONY: clean
clean:
	-rm -f $(EXENAME) $(SRC:.c=.o)

.PHONY: distclean
distclean: clean
	-rm -f *.spec *.pod

.PHONY: realclean
realclean: distclean
	-rm -f *.png_c *.1 Makefile.depend

.PHONY: strip
strip: all
	strip $(EXENAME)

.PHONY: install-files
install-files: all
	install -d              $(DESTDIR)/$(BINDIR)
	install -c $(EXENAME)   $(DESTDIR)/$(BINDIR)
	install -d              $(DESTDIR)/$(MANDIR)/man1
	install -c $(EXENAME).1 $(DESTDIR)/$(MANDIR)/man1

.PHONY: install
install: strip install-files

DISTFILES = $(SRC:.c=.h) $(SRC) $(EXTRAS)
DISTNAME = $(EXENAME)-$(VERSION)

.PHONY: dist
dist: $(DISTFILES)
	rm -rf /tmp/$(DISTNAME)
	mkdir /tmp/$(DISTNAME)
	cp -a $(DISTFILES) /tmp/$(DISTNAME)/
	tar cvjf $(DISTNAME).tar.bz2 -C /tmp $(DISTNAME)/
	rm -rf /tmp/$(DISTNAME)

.PHONY: rpm
rpm: dist $(EXENAME).spec
	rpmbuild -ta $(DISTNAME).tar.bz2

include Makefile.depend
