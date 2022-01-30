DIR_ROOT := $(shell cd ..; pwd)
DIR_SRC := $(DIR_ROOT)/src

VERSION := $(shell cat $(DIR_ROOT)/version)

CC := gcc

VPATH += $(DIR_SRC)
VPATH += $(DIR_ROOT)/vendor/mbed_sha256

INCLUDES += -I$(DIR_ROOT)/include
INCLUDES += -I.
INCLUDES += -I$(DIR_ROOT)/vendor

DEFINES += -DLAPEL_VERSION=\"$(VERSION)\"

CFLAGS := -O0 -Wall -Wextra -ggdb $(INCLUDES) $(DEFINES)
LDFLAGS := -ggdb -lgtest -lpthread -lstdc++ -lm -lutil

SRC += $(notdir $(wildcard $(DIR_SRC)/*)) mbed_sha256.c

OBJ1 := $(SRC:.c=.o)
OBJ := $(OBJ1:.cpp=.o)

all: bin/terminal_example

build/%.o: %.cpp
	@ echo building $@
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.c
	@ echo building $@
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*

sqeaky_clean: clean
	rm -f bin/*

bin/terminal_example:  $(addprefix build/, $(OBJ) terminal_example.o)
	@ echo linking $@
	@ $(CC) $^ -o $@ $(LDFLAGS)

run:
	putty -raw -P 9002

mccabe:
	pmccabe -vt $(shell ls $(DIR_ROOT)/src/*) $(shell ls $(DIR_ROOT)/include/*)

check: CC := clang
check: CFLAGS += --analyze -Xanalyzer -analyzer-output=text
check: $(addprefix build/, $(OBJ))

valgrind: bin/terminal_example
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes --log-file=valgrind.log ./bin/terminal_example


.PHONY: valgrind mccabe
