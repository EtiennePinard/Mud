.PHONY: all mud backends examples tests install clean help

all: mud backends

mud:
	$(MAKE) -C mud all

backends:
	for dir in backends/*; do \
	$(MAKE) -C $$dir all; \
	done

examples:
	for dir in examples/*; do \
	$(MAKE) -C $$dir all; \
	done

tests:
	$(MAKE) -C tests all

install:
	$(MAKE) -C mud install

clean:
	$(MAKE) -C mud clean
	$(MAKE) -C tests clean
	for dir in backends/*; do \
	$(MAKE) -C $$dir clean; \
	done
	for dir in examples/*; do \
	$(MAKE) -C $$dir clean; \
	done

help:
	@echo "make mud      \t - Builds the shared library file for the Mud API"
	@echo "make backends \t - Builds the shared library files for every Mud backend"
	@echo "make examples \t - Builds the executable files for all Mud example projects"
	@echo "make tests    \t - Builds and runs the test for the Mud API"
	@echo "make install  \t - Installs the Mud and all its backend shared library to the system"
	@echo "make clean    \t - Cleans the build artifacts and shared library files for the Mud API, backends, tests and examples"
	@echo "make help     \t - Displays this help message"
