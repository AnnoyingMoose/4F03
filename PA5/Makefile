SHELL = /bin/sh

CC = nvcc
CFLAGS = -arch=compute_30
LDFLAGS =
EXEC = pa5.x
SRCS = pa5.c blur.cu ppmFile.c

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(EXEC)
