all: clean refresh build

cmake_build:
	mkdir cmake_build

refresh: cmake_build
	cd cmake_build;cmake $(CMAKE_FLAGS) ..

build: refresh
	cd cmake_build; make

cmake_build/test_ebnf_ini: build

run_test: cmake_build/test_ebnf_ini
	cmake_build/test_ebnf_ini

clean:
	rm -r -f ./cmake_build

rebuild: clean build
