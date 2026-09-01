NAME = hello_world_sdl
SRC_DIR = src
BUILD_DIR = build

INCLUDES = $(shell pkg-config --cflags mud sdl_backend mud_common mud_widgets)
LIBS = $(shell pkg-config --libs mud sdl_backend mud_common mud_widgets)

include ../../mk/examples.mk
