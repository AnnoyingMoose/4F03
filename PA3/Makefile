SHELL = /bin/sh

CC = gcc
CFLAGS = -Wall
LDFLAGS =
EXECS = client server_append server_verify

.PHONY: all clean

all: $(EXECS)

$(EXECS): %: %.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(EXECS) out.txt
