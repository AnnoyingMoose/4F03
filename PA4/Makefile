SHELL = /bin/sh

CC  = gcc
CXX = g++
CFLAGS   = -Wall -std=c11
CXXFLAGS = -Wall -std=c++14
LDFLAGS =
DEXEC =
FEXEC =

SRCS_C =
SRCS_CXX =
SRCS = $(SRCS_C) $(SRCS_CXX)

OBJS_C   = $(SRCS_C:%.c=%.o)
OBJS_CXX = $(SRCS_CXX:%.cpp=%.o)
OBJS = $(OBJS_C) $(OBJS_CXX)

DEPS_C   = $(SRCS_C:%.c=%.d)
DEPS_CXX = $(SRCS_CXX:%.cpp=%.d)
DEPS = $(DEPS_C) $(DEPS_CXX)

.PHONY: all objects depends final clean

all: $(DEXEC)
objects: $(OBJS)
depends: $(DEPS)

$(DEPS_C): %.d: %.c
	$(CC) $(CFLAGS) -MM -MT '$@ $(@:%.d=%.o)' $< > $@

$(DEPS_CXX): %.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$@ $(@:%.d=%.o)' $< > $@

include $(DEPS)

$(DEXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(OBJS_C): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_CXX): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

final:
	$(CXX) $(CXXFLAGS) -O2 $(SOURCES) $(LDFLAGS) -o $(FEXEC)

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(DEXEC)
