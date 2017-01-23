SHELL = /bin/sh

CC = gcc
CFLAGS = -Wall
LDFLAGS =
EXEC = pa1.x
SOURCES = pa1.c
OBJECTS = $(SOURCES:%.c=%.o)

.PHONY: all objects clean

all: $(EXEC)

objects: $(OBJECTS)

#*.c: *.h
#	touch $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)
