BUILD_DIR = build
RUNNER_NAME = test_runner
RUNNER = $(BUILD_DIR)/$(RUNNER_NAME)

CORE_TEST_DIR = core
BACKEND_SDL_TEST_DIR = backend_sdl
RUNNER_DIR = runner

MUD_DIR = ../mud
BACKEND_SDL_DIR = ../backends/sdl_backend
TEST_BACKEND_DIR = $(CORE_TEST_DIR)/test_backend

.PHONY: all core backend-sdl clean help

all: core backend-sdl

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(RUNNER): $(RUNNER_DIR)/test_runner.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

core: $(RUNNER)
	$(MAKE) -C $(MUD_DIR) DEBUG=1 all
	$(MAKE) -C $(TEST_BACKEND_DIR) DEBUG=1 all
	$(MAKE) -C $(CORE_TEST_DIR) BUILD_DIR=../$(BUILD_DIR)/core all
	@files=$$(find $(BUILD_DIR)/core/ -type f ! -name "*.d"); \
	./$(RUNNER) $$files

backend-sdl: $(RUNNER)
	$(MAKE) -C $(BACKEND_SDL_DIR) DEBUG=1 all
	$(MAKE) -C $(BACKEND_SDL_TEST_DIR) \
		BUILD_DIR=../$(BUILD_DIR)/backend \
		all
	@files=$$(find $(BUILD_DIR)/backend/ -type f ! -name "*.d"); \
	./$(RUNNER) $$files

clean:
	rm -r $(BUILD_DIR)

help:
	@echo "make -C tests core        \t - Runs the tests for the core Mud API"
	@echo "make -C tests backend-sdl \t - Runs the testes for the Mud SDL backend"
	@echo "make -C tests clean       \t - Runs the build directory of the tests"
	@echo "make -C tests help        \t - Prints this help message"