DIR_ROOT := $(shell cd ..; pwd)

CC := gcc

VPATH += $(DIR_ROOT)/src

INCLUDES += -I$(DIR_ROOT)/include
INCLUDES += -I.

CFLAGS := -O0 -Wall -Wextra -ggdb -pthread $(INCLUDES)
LDFLAGS := -ggdb -lgtest -lpthread -lstdc++ -lm

SRC := $(notdir $(wildcard $(DIR_ROOT)/src/*.c)) $(notdir $(wildcard $(DIR_ROOT)/src/*.cpp))

OBJ_INTERMEDIATE := $(SRC:.cpp=.o)
OBJ := $(OBJ_INTERMEDIATE:.c=.o)

TESTS += decoder_test
TESTS += tokenizer_test
TESTS += get_opt_test

LINE := ================================================================

.PHONY: clean all line check

all: $(addprefix bin/, $(TESTS))

line:
	@ echo $(LINE)

build_and_run:
	@ FAIL=0; \
	for tc in $(addprefix bin/, $(TESTS)); do \
		echo $(LINE); \
		echo ""; \
		echo "$$tc:"; \
		echo ""; \
		make clean $$tc \
		&& echo "" \
		&& if ! ./$$tc; then FAIL=1; fi; \
		echo ""; \
	done; \
	exit $$FAIL

build/%.o: %.cpp
	@ echo building $@
	@ mkdir -p build
	@ $(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.c
	@ echo building $@
	@ mkdir -p build
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*

sqeaky_clean: clean
	rm -f bin/*

mccabe:
	pmccabe -vt $(addprefix $(DIR_ROOT)/src/, $(SRC))

check: CC := clang
check: CFLAGS += --analyze -Xanalyzer -analyzer-output=text
check: $(addprefix build/, $(OBJ))

bin/decoder_test: $(addprefix build/,\
		lapel_decoder.o\
		decoder_test.o\
	)
	@ echo linking $@
	@ mkdir -p bin
	@ $(CC) $^ -o $@ $(LDFLAGS)

bin/tokenizer_test: $(addprefix build/,\
		lapel_tokenizer.o\
		tokenizer_test.o\
	)
	@ echo linking $@
	@ mkdir -p bin
	@ $(CC) $^ -o $@ $(LDFLAGS)

bin/get_opt_test: $(addprefix build/,\
		lapel_get_opt.o\
		get_opt_test.o\
	)
	@ echo linking $@
	@ mkdir -p bin
	@ $(CC) $^ -o $@ $(LDFLAGS)
