CC = cc
CFLAGS = -Wall -ggdb -fPIC -Iinclude
LDFLAGS = 

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJECTS_DIR = $(BUILD_DIR)/objects

SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJECTS_DIR)/%.o, $(SRC_FILES))

# chmod 755 libwebc.so
LIB_NAME = $(BUILD_DIR)/output/libwebc.so

# chmod 644 libwebc.a
STATIC_LIB_NAME = $(BUILD_DIR)/output/libwebc.a

# read access for all users
DLL_NAME = $(BUILD_DIR)/output/webc.dll

all: $(BUILD_DIR) shared static dll

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/output
	mkdir -p $(OBJECTS_DIR)/core
	mkdir -p $(OBJECTS_DIR)/ui
	mkdir -p $(OBJECTS_DIR)/actions
	mkdir -p $(OBJECTS_DIR)/server

$(OBJECTS_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

shared: $(BUILD_DIR) $(OBJ_FILES)
	$(CC) -shared -o $(LIB_NAME) $(OBJ_FILES)
	chmod 755 $(LIB_NAME)

static: $(BUILD_DIR) $(OBJ_FILES)
	ar rcs $(STATIC_LIB_NAME) $(OBJ_FILES)
	chmod 644 $(STATIC_LIB_NAME)

dll: $(BUILD_DIR) $(OBJ_FILES)
	$(CC) -shared -o $(DLL_NAME) $(OBJ_FILES) -Wl,--out-implib,$(BUILD_DIR)/output/libwebc.dll.a
	chmod 644 $(DLL_NAME)
	chmod 644 $(BUILD_DIR)/output/libwebc.dll.a

clean:
	rm -rf $(BUILD_DIR)

compile_commands.json: $(SRC_FILES)
	bear -- make

.PHONY: all clean shared static dll
