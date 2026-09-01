RULE_DIR ?= ../../mk

include $(RULE_DIR)/rules.mk

BUILD_DIR ?= build
TEST_SRC_DIR ?= src

TEST_SRC := $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_BIN := $(patsubst $(TEST_SRC_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SRC))

.PHONY: all clean help

all: $(BUILD_DIR) $(TEST_BIN)

$(BUILD_DIR)/%: $(TEST_SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(LIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make all   \t - Compiles the tests in the $(TEST_SRC_DIR) directory"
	@echo "make clean \t - Removes the $(BUILD_DIR) directory"
	@echo "make help  \t - Displays this help message"
