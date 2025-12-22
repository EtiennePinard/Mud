include ../../.env

DEBUG = 1

SRC_DIR = src
BUILD_DIR ?= ../build/core

INCLUDES = -I../../mud/include -Itest_backend/include

MUD_LIB_DIR = $(abspath ../../mud/build)
TEST_BACKEND_LIB_DIR = $(abspath test_backend/build)
LIBS = -L$(MUD_LIB_DIR) -lmud -L$(TEST_BACKEND_LIB_DIR) -ltest_backend -Wl,-rpath,$(MUD_LIB_DIR):$(TEST_BACKEND_LIB_DIR)

RULE_DIR = ../../mk
include $(RULE_DIR)/tests.mk
