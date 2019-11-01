mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* test/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: test
test: build
	TEST_FS_ROOT=${mkfile_dir}test_filesystem_roots/one/ ./build/test

.PHONY: run
run: build
	./build/monitor

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build
