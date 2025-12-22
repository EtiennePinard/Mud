include ../../../.env

NAME = test_backend
SRC_DIR = src
BUILD_DIR = build

INCLUDES = \
 -Iinclude \
 -I"${MUD_INCLUDE_PATH}"

PREFIX ?= /usr/local

RULE_DIR = ../../../mk
include $(RULE_DIR)/library.mk

# We do not want to install this backend
install:
	@echo "You cannot install the test backend."
	@echo "Ignore this message and the overriding recipe warning if you are simply running sudo make install"