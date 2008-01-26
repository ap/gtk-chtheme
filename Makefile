GCC = cc
PREFIX=/usr
CFLAGS = -O2 -Wall $(shell pkg-config --cflags gtk+-2.0)
LIBS = $(shell pkg-config --libs gtk+-2.0)
CFLAGS += -DGTK_DISABLE_BROKEN -DGTK_DISABLE_DEPRECATED -DVERSION=\"$(VERSION)\"

PROGNAME = gtk-chtheme
VERSION = 0.1
all: $(PROGNAME)

$(PROGNAME): $(PROGNAME).c
	${GCC} -o $(PROGNAME) $(PROGNAME).c ${CFLAGS} ${LIBS}

clean:
	-rm -f $(PROGNAME) *~
	
install: all
	strip $(PROGNAME)
	mkdir -p ${PREFIX}/bin
	install -c $(PROGNAME) ${PREFIX}/bin

dist: clean
	rm -rf /tmp/$(PROGNAME)-$(VERSION)
	tar cjf ../$(PROGNAME)-$(VERSION).tar.bz2 .
