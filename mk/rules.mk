CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -MMD -MP

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -DDEBUG
else
	CFLAGS += -O3 -DNDEBUG
endif

SRC_DIR ?= src
BUILD_DIR ?= build

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC := $(call rwildcard,$(SRC_DIR),*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

-include $(OBJ:.o=.d)
