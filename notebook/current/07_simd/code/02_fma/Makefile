include ../common.mk

CXXFLAGS := ${CXXFLAGS} -mfma

BIN := fma

.PHONY: default
default: ${BIN}

simd.o: simd.cpp fma.hpp
	${CXX} -std=c++17 -g -mfma -O3 -c -o $@ $<

loop.o: loop.cpp fma.hpp
	${CXX} -std=c++17 -g -O3 -c -o $@ $<

fma.o: fma.cpp fma.hpp StopWatch.hpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

fma: fma.o simd.o loop.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

.PHONY: run
run: ${BIN}
	./${BIN}

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BIN}
