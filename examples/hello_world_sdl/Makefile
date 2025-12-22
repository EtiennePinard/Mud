include ../../.env

NAME = hello_world_sdl
SRC_DIR = src
BUILD_DIR = build

COMMON_INCLUDE_PATH = $(abspath ../common/include)
COMMON_LIBRARY_PATH = $(abspath ../common/build)
COMMON_PATH = $(abspath ../common)

INCLUDES = \
 -I"$(COMMON_INCLUDE_PATH)" \
 -I"${MUD_INCLUDE_PATH}" \
 -I"${SDL_BACKEND_INCLUDE_PATH}" \
 -I"${SDL_INCLUDE_PATH}" \
 -I"${SDL_TTF_INCLUDE_PATH}"

LIBS = \
 -L$(COMMON_LIBRARY_PATH) -lmud_common \
 -L$(MUD_LIBRARY_PATH) -lmud \
 -L$(SDL_BACKEND_LIBRARY_PATH) -lsdl_backend \
 -L$(SDL_LIBRARY_PATH) -L$(SDL_TTF_LIBRARY_PATH) -L$(SDL_IMAGE_LIBRARY_PATH) \
 -lSDL3 -lSDL3_ttf -lSDL3_image \
 -Wl,-rpath,$(COMMON_LIBRARY_PATH)

include ../../mk/examples.mk