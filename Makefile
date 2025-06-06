# TODO: find out way to make following work - it does bring in the Makefile,
# but it doesn't have correct context for the includes the Makefile brings in
# itself
# -include ./build/Makefile

all: cmake
.PHONY : all

cmake:
	cmake -S . -B build

.PHONY : cmake

run:
	cd build && make run

.PHONY : run
