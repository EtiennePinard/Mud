include ../../.env

NAME = sdl_backend
SRC_DIR = src
BUILD_DIR = build

INCLUDES = \
 -Iinclude \
 -I"${MUD_INCLUDE_PATH}" \
 -I"${SDL_INCLUDE_PATH}" \
 -I"${SDL_TTF_INCLUDE_PATH}" \
 -I"${SDL_IMAGE_INCLUDE_PATH}"

PREFIX ?= /usr/local

include ../../mk/library.mk
