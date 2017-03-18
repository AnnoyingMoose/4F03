SHELL = /bin/sh

CC = gcc
CFLAGS = -Wall -fopenmp
LDFLAGS =
EXEC = pa3.x
SOURCES = pa3.c
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
