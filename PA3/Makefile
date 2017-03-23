#
# Makefile
# --------
#
# For SFWR ENG 4F03 PA3 (Winter 2017)
#
# Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
# James Lee            (leejr@mcmaster.ca, MSID#001318125)
#

SHELL = /bin/sh

CC = gcc
CFLAGS = -Wall -fopenmp
LDFLAGS =
EXECS = client server_append server_verify

SOURCES =                \
	client.c             \
	server_append.c      \
	server_append_svc.c  \
	server_append_clnt.c \
	server_verify.c      \
	server_verify_svc.c  \
	server_verify_clnt.c \
	property.c

OBJECTS = $(SOURCES:%.c=%.o)

.PHONY: all objects clean

all: $(EXECS)
objects: $(OBJECTS)

client: client.o server_append_clnt.o server_verify_clnt.o property.o
server_append: server_append.o server_append_svc.o property.o
server_verify: server_verify.o server_verify_svc.o

$(EXECS):
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

client.o server_append.o server_append_svc.o server_append_clnt.o: server_append.h
client.o server_verify.o server_verify_svc.o server_verify_clnt.o: server_verify.h
client.o server_append.o property.o: property.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

server_%_svc.c server_%_clnt.c server_%.h: server_%.x
	rpcgen $<

clean:
	rm -f $(EXECS) $(OBJECTS) out.txt
