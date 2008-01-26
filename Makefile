PREFIX=/usr
LIBS = $(shell pkg-config --libs gtk+-2.0)
CFLAGS += -Wall
CFLAGS += $(shell pkg-config --cflags gtk+-2.0) -DGTK_DISABLE_BROKEN -DGTK_DISABLE_DEPRECATED

PROJNAME = "Gtk+ 2.0 Change Theme"
EXENAME = gtk-chtheme
VERSION = 0.2

CFLAGS += -DPROJNAME=\"$(PROJNAME)\" -DVERSION=\"$(VERSION)\"

AUXILIARY = stock themeutil theme demo about

all: $(EXENAME)

$(EXENAME): main.c $(addsuffix .c, $(AUXILIARY)) $(addsuffix .h, $(AUXILIARY))
	$(CC) -o $(EXENAME) $(filter %.c, $^) $(CFLAGS) $(LIBS)

clean:
	-rm -f $(EXENAME)
	
install: all
	strip $(EXENAME)
	mkdir -p ${PREFIX}/bin
	install -c $(EXENAME) ${PREFIX}/bin

dist: clean
	rm -rf /tmp/$(EXENAME)-$(VERSION)
	cp -a . /tmp/$(EXENAME)-$(VERSION)
	tar cvjf ../$(EXENAME)-$(VERSION).tar.bz2 -C /tmp $(EXENAME)-$(VERSION)
