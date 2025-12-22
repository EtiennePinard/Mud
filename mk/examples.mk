include ../../mk/rules.mk
CFLAGS += -fPIE

APP_PATH := $(BUILD_DIR)/$(NAME)

ASSET_SRC = ../assets
ASSET_DST = $(BUILD_DIR)/assets

.PHONY: all assets run clean

all: mud_common $(APP_PATH)

$(APP_PATH): $(OBJ)
	$(CC) $^ $(LIBS) -o $@

$(ASSET_DST): $(ASSET_SRC) | $(BUILD_DIR)
    # Removing the asset directory because Windows
	@if [ -e $@ ]; then rm -rf $@; fi
    # The -T is to avoid creating recursive symlinks
	@ln -sfT $(realpath $<) $@

assets: $(ASSET_DST)

run: all assets
	cd build && ./$(NAME)

mud_common:
	$(MAKE) -C $(COMMON_PATH) all

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make all   \t - Creates the executable file $(APP_PATH)"
	@echo "make run   \t - Runs the $(NAME) executable"
	@echo "make clean \t - Removes the $(BUILD_DIR) directory"