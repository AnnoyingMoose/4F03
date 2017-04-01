SHELL = /bin/sh

CC = mpicc
CFLAGS = -Wall
LDFLAGS =
EXEC = pa4.x
SRCS = pa4.c blur.c

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(EXEC)
