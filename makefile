# Global settings

MAKEFLAGS += --no-builtin-rules --warn-undefined-variables
.SUFFIXES:

# ==== Config ====

PROFILE ?= local
LIBC    ?= glibc
CC ?= gcc

LIB_NAME := esclib
TESTER_NAME := tester

BASE_CFLAGS := -Wall -Wextra -Werror -Iinclude
DEV_CFLAGS  := -Og -g3 -fno-omit-frame-pointer -fsanitize=address,undefined
REL_CFLAGS  := -O2
PIC_FLAG    := -fPIC

AR      := ar
ARFLAGS := rcs

# ==== Platform ====

ifeq ($(PROFILE),windows)
CC := x86_64-w64-mingw32-gcc
DLL_EXT := .dll
SO_EXT  :=
else
DLL_EXT :=
SO_EXT  := .so
endif

ifeq ($(PROFILE),windows-static)
CC := x86_64-w64-mingw32-gcc
DLL_EXT := .dll
SO_EXT  :=
override LDFLAGS += -static -static-libgcc
endif

ifeq ($(LIBC),static-musl)
LIBC_CFLAGS  := -static
LIBC_LDFLAGS := -static
else
LIBC_CFLAGS  :=
LIBC_LDFLAGS :=
endif

override CFLAGS += $(BASE_CFLAGS) $(REL_CFLAGS) $(LIBC_CFLAGS)
LDFLAGS ?= $(LIBC_LDFLAGS)

# ==== Paths ====

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := compiled

OBJ_SUBDIR := $(OBJ_DIR)/$(PROFILE)-$(LIBC)
BIN_SUBDIR := $(PROFILE)

TEST_DIR := tester
TEST_SRC := $(shell find $(TEST_DIR)/src -name '*.c')
TEST_OBJ := $(patsubst $(TEST_DIR)/src/%.c,$(OBJ_SUBDIR)/$(TESTER_NAME)_%.o,$(TEST_SRC))
TEST_BIN := $(TESTER_NAME)/$(BIN_DIR)/$(TESTER_NAME)


STATIC_LIB := $(BIN_DIR)/$(BIN_SUBDIR)/lib$(LIB_NAME).a
SHARED_LIB := $(BIN_DIR)/$(BIN_SUBDIR)/lib$(LIB_NAME)$(SO_EXT)
WINDOWS_DLL := $(BIN_DIR)/$(BIN_SUBDIR)/lib$(LIB_NAME)$(DLL_EXT)

# ==== Sources ====

SRC := $(sort $(shell find $(SRC_DIR) -name '*.c'))
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_SUBDIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

.PHONY: all tester build static shared install clean clean-all \
	local-build san-build check-build \
	docker-windows docker-windows-static \
	docker-bleeding docker-normal docker-stable \
	docker-bleeding-musl docker-normal-musl docker-stable-musl \
	docker-static-musl

# ==== Targets ====

all: local-build docker-bleeding docker-normal docker-stable \
     docker-bleeding-musl docker-normal-musl docker-stable-musl \
     docker-static-musl docker-windows

build: static shared

local-build:
	$(MAKE) PROFILE=local LIBC=glibc build

$(OBJ_SUBDIR)/$(TESTER_NAME)_%.o: $(TEST_DIR)/src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(TEST_BIN): static shared $(TEST_OBJ)
	@mkdir -p $(@D)
	$(CC) $(TEST_OBJ) -L$(BIN_DIR)/$(BIN_SUBDIR) -l$(LIB_NAME) -o $@

tester: $(TEST_BIN)

san-build:
	$(MAKE) PROFILE=san LIBC=glibc CFLAGS="$(BASE_CFLAGS) $(DEV_CFLAGS) $(PIC_FLAG)" LDFLAGS="-fsanitize=address,undefined" build

check-build:
	$(MAKE) PROFILE=check LIBC=glibc build

# ==== Static library ====

static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJ)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $(OBJ)

# ==== Shared library (Linux) ====

ifneq ($(LIBC),static-musl)
shared: $(SHARED_LIB)
ifneq ($(DLL_EXT),)
shared: $(WINDOWS_DLL)
endif
endif

ifneq ($(SO_EXT),)
ifneq ($(LIBC),static-musl)
$(SHARED_LIB): $(OBJ)
	@mkdir -p $(@D)
	$(CC) -shared $(OBJ) -o $@ $(LDFLAGS)
endif
endif

# ==== DLL (Windows) ====

ifneq ($(DLL_EXT),)
$(WINDOWS_DLL): $(OBJ)
	@mkdir -p $(@D)
	$(CC) -shared $(OBJ) -o $@
endif

# ==== Object build ====

$(OBJ_SUBDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(PIC_FLAG) -MMD -MP -c $< -o $@

-include $(DEP)

# ==== Install ====

PREFIX  ?= /usr/local
DESTDIR ?=

install: static shared
	install -Dm644 $(STATIC_LIB) $(DESTDIR)$(PREFIX)/lib/lib$(LIB_NAME).a
	install -Dm644 include/$(LIB_NAME).h $(DESTDIR)$(PREFIX)/include/$(LIB_NAME).h
ifneq ($(SO_EXT),)
	install -Dm755 $(SHARED_LIB) $(DESTDIR)$(PREFIX)/lib/lib$(LIB_NAME).so
endif

# ==== Docker ====

UID := $(shell id -u)
GID := $(shell id -g)
DOCKER_USER := --rm -u $(UID):$(GID) -v "$(shell pwd)":/src -w /src
DOCKER_ROOT := --rm -v "$(shell pwd)":/src -w /src
FIX_PERMS   := ; chown -R $(UID):$(GID) $(OBJ_DIR) $(BIN_DIR)

docker-windows:
	docker run $(DOCKER_ROOT) debian:stable-slim sh -c "apt-get update && apt-get install -y --no-install-recommends mingw-w64 make gcc && $(MAKE) PROFILE=windows LIBC=glibc build $(FIX_PERMS)"

docker-windows-static:
	docker run $(DOCKER_ROOT) debian:stable-slim sh -c "apt-get update && apt-get install -y --no-install-recommends mingw-w64 make gcc && $(MAKE) PROFILE=windows-static LIBC=glibc build $(FIX_PERMS)"

docker-bleeding:
	docker run $(DOCKER_USER) gcc:latest $(MAKE) PROFILE=bleeding LIBC=glibc build

docker-normal:
	docker run $(DOCKER_USER) gcc:11 $(MAKE) PROFILE=normal LIBC=glibc build

docker-stable:
	docker run $(DOCKER_USER) gcc:7 $(MAKE) PROFILE=stable LIBC=glibc build

docker-bleeding-musl:
	docker run $(DOCKER_ROOT) alpine:latest sh -c "apk add --no-cache build-base musl-dev && $(MAKE) PROFILE=bleeding LIBC=musl build $(FIX_PERMS)"

docker-normal-musl:
	docker run $(DOCKER_ROOT) alpine:latest sh -c "apk add --no-cache build-base musl-dev && $(MAKE) PROFILE=normal LIBC=musl build $(FIX_PERMS)"

docker-stable-musl:
	docker run $(DOCKER_ROOT) alpine:3.19 sh -c "apk add --no-cache build-base musl-dev && $(MAKE) PROFILE=stable LIBC=musl build $(FIX_PERMS)"

docker-static-musl:
	docker run $(DOCKER_ROOT) alpine:3.19 sh -c "apk add --no-cache build-base musl-dev && $(MAKE) PROFILE=static LIBC=static-musl build $(FIX_PERMS)"

# ==== Clean ====

clean:
	rm -rf $(OBJ_SUBDIR)

clean-all:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
