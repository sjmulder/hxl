DESTDIR   ?=
PREFIX    ?= /usr/local
MANPREFIX ?= $(PREFIX)/man

DOCKER ?= sudo docker

CFLAGS += -Wall -Wextra
CFLAGS += -O3

all: hxl

install: hxl
	install -d $(DESTDIR)$(PREFIX)/bin $(DESTDIR)$(MANPREFIX)/man1
	install -m755 hxl   $(DESTDIR)$(PREFIX)/bin/
	install -m644 hxl.1 $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/hxl
	rm -f $(DESTDIR)$(MANPREFIX)/man1/hxl.1

clean:
	rm -f hxl hxl.static hxl.o

docker: hxl.static
	$(DOCKER) build -t hxl .

hxl.static: hxl.o
	$(CC) $(LDFLAGS) -static -o hxl.static hxl.o $(LDLIBS)

.PHONY: all docker clean
