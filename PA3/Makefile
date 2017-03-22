SHELL = /bin/sh

CC = gcc
CFLAGS = -Wall -fopenmp
LDFLAGS =
EXECS = client server_append server_verify
SOURCES = client.c server_append.c server_verify.c property.c
OBJECTS = $(SOURCES:%.c=%.o)

.PHONY: all objects clean

all: $(EXECS)
objects: $(OBJECTS)

client: client.o property.o
server_append: server_append.o property.o
server_verify: server_verify.o

$(EXECS):
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

client.o server_append.o property.o: property.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS) $(OBJECTS) out.txt
