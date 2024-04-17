.POSIX:

include config.mk

COM =

all: sdns

$(COM:=.o): config.mk sdns.h
sdns.o: sdns.c sdns.h config.h config.mk $(REQ:=.h)

.c.o:
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

config.h:
	cp config.def.h $@

sdns: sdns.o $(COM:=.o) $(REQ:=.o)
	$(CC) -o $@ $(LDFLAGS) $(COM:=.o) $(REQ:=.o) sdns.o $(LDLIBS)

clean:
	rm -f sdns sdns.o $(COM:=.o) $(REQ:=.o) sdns-${VERSION}.tar.gz

dist:
	rm -rf "sdns-$(VERSION)"
	mkdir -p "sdns-$(VERSION)/components"
	cp -R LICENSE Makefile README config.mk config.def.h \
	      arg.h sdns.h sdns.c $(REQ:=.c) $(REQ:=.h) \
	      sdns.1 "sdns-$(VERSION)"
	cp -R $(COM:=.c) "sdns-$(VERSION)/components"
	tar -cf - "sdns-$(VERSION)" | gzip -c > "sdns-$(VERSION).tar.gz"
	rm -rf "sdns-$(VERSION)"

install: all
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	cp -f sdns "$(DESTDIR)$(PREFIX)/bin"
	chmod 755 "$(DESTDIR)$(PREFIX)/bin/sdns"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/sdns
