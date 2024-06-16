CC = cc
CFLAGS = -Wall -fPIC -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# List all the source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Generate the corresponding object file names
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

LIB_SOURCES = $(SRC_FILES)
LIB_OBJECTS = $(OBJ_FILES)
LIB_NAME = webc.so

# Default target, build the executable
all: $(BUILD_DIR) library

# Rule to create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to build object files from source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

library: $(LIB_OBJECTS)
	$(CC) -shared -o $(LIB_NAME) $(LIB_OBJECTS)

# Clean rule to remove generated files
clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME)

compile_commands.json: $(SRC_FILES)
	bear -- make

# Phony target to avoid conflicts with file names
.PHONY: all clean library

