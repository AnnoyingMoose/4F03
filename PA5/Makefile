SHELL = /bin/sh

CC = nvcc
CFLAGS = -arch=sm_20
LDFLAGS =
EXEC = pa5
SRCS = pa5.cu blur.cu ppmFile.cu

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(EXEC)
