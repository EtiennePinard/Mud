RULE_DIR ?= ../../mk

include $(RULE_DIR)/rules.mk

BUILD_DIR ?= build
TEST_SRC_DIR ?= src

TEST_SRC := $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_BIN := $(patsubst $(TEST_SRC_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SRC))

.PHONY: all compile-tests clean

all: compile-tests

compile-tests: $(BUILD_DIR) $(TEST_BIN)

$(BUILD_DIR)/%: $(TEST_SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(LIBS) -o $@

help:
	@echo "make compile-tests \t - compile the tests"