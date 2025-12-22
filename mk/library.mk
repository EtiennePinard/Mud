RULE_DIR ?= ../../mk

include $(RULE_DIR)/rules.mk
CFLAGS += -fPIC

LIB_NAME := lib$(NAME).so
LIB_PATH := $(BUILD_DIR)/$(LIB_NAME)

.PHONY: all install clean

all: $(LIB_PATH)

$(LIB_PATH): $(OBJ)
	$(CC) -shared -o $@ $^

install:
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -d $(PREFIX)/lib/pkgconfig

	install -m 755 $(LIB_PATH) $(PREFIX)/lib
	install -m 644 include/*.h $(PREFIX)/include
	install -m 644 $(NAME).pc $(PREFIX)/lib/pkgconfig

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make all     \t - Creates the shared library file at $(LIB_PATH)"
	@echo "make install \t - Installs the $(NAME) headers and shared library to $(PREFIX)/lib and $(PREFIX)/include"
	@echo "make clean   \t - Removes the $(BUILD_DIR) directory"