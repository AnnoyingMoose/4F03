SHELL = /bin/sh

CC = gcc
CFLAGS = -Wall -fopenmp
LDFLAGS =
EXEC = pa2.x
SOURCES = pa2.c
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
	rm -f $(OBJECTS) $(EXEC) out.txt
