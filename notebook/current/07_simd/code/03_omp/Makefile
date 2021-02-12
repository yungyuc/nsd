include ../common.mk

BIN := omp

ifeq ($(UNAME_S),Darwin)
# https://iscinumpy.gitlab.io/post/omp-on-high-sierra/
CXXFLAGS := -Xpreprocessor -fopenmp
LINKFLAGS := -lomp
CXX = clang++
endif

ifeq ($(UNAME_S),Linux)
CXXFLAGS := -Wl,--no-as-needed -fopenmp
LINKFLAGS := -fopenmp
CXX = g++
endif

CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -g -O3 \
	${FLAGS}

.PHONY: default
default: ${BIN}

omp.o: omp.cpp Makefile
	${CXX} ${CXXFLAGS} -c -o $@ $<

omp: omp.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

.PHONY: run
run: ${BIN}
	./${BIN}

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BIN}
