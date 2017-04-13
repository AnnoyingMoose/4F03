SHELL = /bin/sh

CC = nvcc
CFLAGS = -Wall
LDFLAGS =
EXEC = pa5.x
SRCS = pa5.c blur.c ppmFile.c

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(EXEC)
