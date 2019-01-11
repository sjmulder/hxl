DOCKER ?= sudo docker

CFLAGS += -Wall -Wextra
CFLAGS += -O2

all: hxl

docker: hxl.static
	$(DOCKER) build -t hxl .

clean:
	rm -f hxl hxl.static hxl.o

hxl.static: hxl.o
	$(CC) $(LDFLAGS) -static -o hxl.static hxl.o $(LDLIBS)

.PHONY: all docker clean
