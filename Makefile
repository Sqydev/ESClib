# Paths
SRC_DIR := src/core
OBJ_DIR := obj
LIB_DIR_LINUX := lib/linux
LIB_DIR_WINDOWS := lib/windows
TESTER_SRC := tester/src
TESTER_BIN := tester/compiled

# Libfile's names
LIB_NAME := libTuiUtils
STATIC_LIN := $(LIB_NAME).a
SHARED_LIN := $(LIB_NAME).so
SHARED_WIN := $(LIB_NAME).dll
STATIC_WIN := $(LIB_NAME).lib

# Compilers & Compilers Flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -fPIC -O2
LDFLAGS :=
WIN_CC := x86_64-w64-mingw32-gcc

# SRC files and obj files
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# =====================================================
# Main thingies
# =====================================================

.PHONY: all so a dll lib linux win full tester clean

all: full

# ------------------ Linux ------------------
so: $(LIB_DIR_LINUX)/$(SHARED_LIN)
a: $(LIB_DIR_LINUX)/$(STATIC_LIN)
linux: so a

# ------------------ Windows ------------------
dll: $(LIB_DIR_WINDOWS)/$(SHARED_WIN)
lib: $(LIB_DIR_WINDOWS)/$(STATIC_WIN)
win: dll lib

# ------------------ Full ------------------
full: linux win

# ------------------ Tester ------------------
tester: $(TESTER_BIN)/$(SHARED_LIN)
	@mkdir -p $(TESTER_BIN)
	$(CC) $(CFLAGS) $(TESTER_SRC)/*.c -L$(TESTER_BIN) -lTuiUtils -o $(TESTER_BIN)/tester -Wl,-rpath,$(abspath $(TESTER_BIN))

# Tester's .so builder, so .so of tester is in tester/compiled not lib/
$(TESTER_BIN)/$(SHARED_LIN): $(OBJS)
	@mkdir -p $(TESTER_BIN)
	$(CC) -shared -o $@ $(OBJS)

# =====================================================
# Building thingies
# =====================================================

# Linux .so
$(LIB_DIR_LINUX)/$(SHARED_LIN): $(OBJS)
	@mkdir -p $(LIB_DIR_LINUX)
	$(CC) -shared -o $@ $(OBJS)

# Linux .a
$(LIB_DIR_LINUX)/$(STATIC_LIN): $(OBJS)
	@mkdir -p $(LIB_DIR_LINUX)
	ar rcs $@ $(OBJS)

# Windows .dll
$(LIB_DIR_WINDOWS)/$(SHARED_WIN): $(OBJS)
	@mkdir -p $(LIB_DIR_WINDOWS)
	$(WIN_CC) -shared -o $@ $(OBJS)

# Windows .lib
$(LIB_DIR_WINDOWS)/$(STATIC_WIN): $(OBJS)
	@mkdir -p $(LIB_DIR_WINDOWS)
	ar rcs $@ $(OBJS)

# =====================================================
# Obj laws
# =====================================================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# =====================================================
# Clean
# =====================================================
clean:
	rm -rf $(OBJS) $(LIB_DIR_LINUX) $(LIB_DIR_WINDOWS) $(TESTER_BIN)
