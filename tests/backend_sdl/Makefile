include ../../.env

DEBUG = 1

SRC_DIR = src
BUILD_DIR ?= ../build/backend

INCLUDES = -I../../mud/include -I../../backend/sdl_backend/include

MUD_LIB_DIR = $(abspath ../../mud/build)
SDL_BACKEND_LIB_DIR = $(abspath ../../backends/sdl_backend/build)

LIBS = -L$(MUD_LIB_DIR) -lmud \
   -L$(SDL_BACKEND_LIB_DIR) -lsdl_backend \
   -L$(SDL_LIBRARY_PATH) -L$(SDL_TTF_LIBRARY_PATH) -L$(SDL_IMAGE_LIBRARY_PATH) \
   -lSDL3 -lSDL3_ttf -lSDL3_image \
   -Wl,-rpath,$(MUD_LIB_DIR):$(SDL_BACKEND_LIB_DIR)

RULE_DIR = ../../mk
include $(RULE_DIR)/tests.mk