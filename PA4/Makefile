SHELL = /bin/sh

CC = gcc
CFLAGS = -Wall
LDFLAGS =
EXEC = pa4.x
SRCS = pa4.c blur.c ppmFile.c

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(EXEC)
