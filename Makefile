all: build

build: install

rebuild: clean install

install:
	cmake -S . -B ./build
	cmake --build ./build

unistall:
	find ./ -name "build" -type d -exec rm -rf {} +

clean: unistall
	rm -rf ./lib/*

cppcheck:
	@cd build/; make cppcheck

clang-format:
	@cd build/; make clang-format

tests: build
	cmake -S ./test -B ./test/build
	cmake --build ./test/build
	cd ./test/build/; ./TEST

.PHONY: all build rebuild unistall clean cppcheck clang-format tests