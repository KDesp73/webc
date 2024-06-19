CC = cc
CFLAGS = -Wall -ggdb -fPIC -Iinclude
LDFLAGS = 

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# chmod 755 libwebc.so
LIB_NAME = libwebc.so

# chmod 644 libwebc.a
STATIC_LIB_NAME = libwebc.a

# read access for all users
DLL_NAME = webc.dll

all: library

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/core
	mkdir -p $(BUILD_DIR)/ui
	mkdir -p $(BUILD_DIR)/actions
	mkdir -p $(BUILD_DIR)/server

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

library: $(BUILD_DIR) $(OBJ_FILES)
	$(CC) -shared -o $(LIB_NAME) $(OBJ_FILES)

static_library: $(BUILD_DIR) $(OBJ_FILES)
	ar rcs $(STATIC_LIB_NAME) $(OBJ_FILES)

dll: $(BUILD_DIR) $(OBJ_FILES)
	$(CC) -shared -o $(DLL_NAME) $(OBJ_FILES) -Wl,--out-implib,libwebc.dll.a

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME) $(STATIC_LIB_NAME) $(DLL_NAME) libwebc.dll.a

compile_commands.json: $(SRC_FILES)
	bear -- make

.PHONY: all clean library static_library dll
