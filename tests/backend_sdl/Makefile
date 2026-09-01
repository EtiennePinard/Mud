DEBUG = 1

SRC_DIR = src
BUILD_DIR ?= ../build/backend

INCLUDES = $(shell pkg-config --cflags mud) -I../../backend/sdl_backend/include

SDL_BACKEND_LIB_DIR = $(abspath ../../backends/sdl_backend/build)

LIBS = $(shell pkg-config --libs mud) \
   $(SDL_BACKEND_LIB_DIR)/libsdl_backend.so \
   $(shell pkg-config --libs sdl3 sdl3-ttf sdl3-image) \
   -Wl,-rpath,$(SDL_BACKEND_LIB_DIR)

RULE_DIR = ../../mk
include $(RULE_DIR)/tests.mk
