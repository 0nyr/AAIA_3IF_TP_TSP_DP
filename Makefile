# echo -e colors
# WARN: don't put " and use the echo command, not echo -e
LIGHT_ORANGE_COLOR=\e[38;5;216m
TURQUOISE_COLOR=\e[38;5;43m
LIGHT_BLUE_COLOR=\e[38;5;153m
RED_COLOR=\e[38;5;196m
NO_COLOR=\e[0m

# vars
ECHO = @echo # @echo hides this command in terminal, not its output

BIN=bin/main
INCLUDE=inc/

CC=gcc # Use gcc for C
GDB_DEBUGGER_FLAGS=-g
PERSONAL_COMPIL_FLAGS=-D DEBUG # use own flags, see util.h
CFLAGS=-I $(INCLUDE) -march=native -O3 #$(PERSONAL_COMPIL_FLAGS) $(GDB_DEBUGGER_FLAGS)
LDLIBS=-lm # -lm for math.h
LDFLAGS=-Wall -std=c11 # Adjusted for C, -std=c11 for C11 standard

SRCS=$(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard *.c) # Changed to .c
OBJS=$(SRCS:src/%.c=obj/%.o) # Changed to .c

# provide a list or arguments to the executable
EXE_ARGS=4

# targets
# set default target : https://stackoverflow.com/questions/2057689/how-does-make-app-know-default-target-to-build-if-no-target-is-specified
.DEFAULT_GOAL := default
.PHONY: default build clean run rebuild rr ww dirs clear

default: build

# build: $(BIN)

# $(BIN): $(OBJS)
# 	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# obj/%.o: src/%.c # Changed to .c
# 	mkdir -p $(dir $@)
# 	$(CC) -o $@ -c $^ $(CFLAGS)

tsp: src/tsp.c
	$(ECHO) "$(LIGHT_ORANGE_COLOR)*** Compiling tsp.c *** $(NO_COLOR)"
	$(CC) -o bin/tsp src/tsp.c $(LDFLAGS) $(LDLIBS)

tspO3: src/tsp.c
	$(ECHO) "$(LIGHT_ORANGE_COLOR)*** Compiling tsp.c with O3 *** $(NO_COLOR)"
	$(CC) -o bin/tspO3 src/tsp.c $(CFLAGS) $(LDFLAGS) $(LDLIBS)

TSPnaif: src/TSPnaif.c
	$(ECHO) "$(LIGHT_ORANGE_COLOR)*** Compiling TSPnaif.c *** $(NO_COLOR)"
	$(CC) -o bin/TSPnaif src/TSPnaif.c $(LDFLAGS) $(LDLIBS)

TSPnaifO3: src/TSPnaif.c
	$(ECHO) "$(LIGHT_ORANGE_COLOR)*** Compiling TSPnaif.c with O3 *** $(NO_COLOR)"
	$(CC) -o bin/TSPnaifO3 src/TSPnaif.c $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf bin/* obj/*

run: $(BIN)
	$(ECHO) "$(TURQUOISE_COLOR)*** Executing code *** $(NO_COLOR)"
	./$(BIN) $(EXE_ARGS)


# Determine this makefile's path.
# Be sure to place this BEFORE `include` directives, if any.
THIS_FILE := $(lastword $(MAKEFILE_LIST))

rebuild:
	@$(MAKE) -f $(THIS_FILE) clean
	@$(MAKE) -f $(THIS_FILE) build

rr: # rebuild and rerun
	@$(MAKE) -f $(THIS_FILE) clean
	@$(MAKE) -f $(THIS_FILE) build
	@$(MAKE) -f $(THIS_FILE) run

ww: # where and what
	pwd
	ls -alt

dirs:
	mkdir -p bin/
	mkdir -p obj/

clear: # alias of clean
	@$(MAKE) -f $(THIS_FILE) clean

# special rule for running python
py:
	python3 python/main.py
