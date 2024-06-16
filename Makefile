CC = cc
CFLAGS = -Wall -ggdb -fPIC -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# List all the source files
SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))

# Generate the corresponding object file names
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Target: the final executable (FOR TESTING)
TARGET = webc 
LIB_NAME = libwebc.so

# Default target, build the executable
all: $(BUILD_DIR) library

# Rule to create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to build the executable
$(TARGET): $(OBJ_FILES) $(BUILD_DIR)/main.o	
	$(CC) -o $@ $^ $(LFLAGS)

# Rule to build object files from source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

library: $(OBJ_FILES)
	$(CC) -shared -o $(LIB_NAME) $(OBJ_FILES)

# Clean rule to remove generated files
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(LIB_NAME)

compile_commands.json: $(SRC_FILES)
	bear -- make

# Phony target to avoid conflicts with file names
.PHONY: all clean library

