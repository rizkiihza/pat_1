.PHONY: all clean run

CC = gcc

BUILD_DIR = .
BUILD_FILE = $(BUILD_DIR)/server

SRC_DIR = .
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')

CFLAGS = -lev
ifdef VERBOSE
CFLAGS += -DVERBOSE=1
endif

all: run

clean:
	@rm -f $(BUILD_FILE)

run: server
	@./$(BUILD_FILE)

server: server.c router.* util.*
	@$(CC) -o $(BUILD_FILE) $(SRC_FILES) $(CFLAGS)
