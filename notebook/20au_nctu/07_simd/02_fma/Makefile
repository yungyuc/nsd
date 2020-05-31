FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -g -O3 -m64 -mfma \
	${FLAGS}

LINKFLAGS := \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \

BINS := fma

.PHONY: default
default: ${BINS}

simd.o: simd.cpp fma.hpp
	${CXX} -std=c++17 -g -mfma -O3 -c -o $@ $<

loop.o: loop.cpp fma.hpp
	${CXX} -std=c++17 -g -O3 -c -o $@ $<

fma.o: fma.cpp fma.hpp StopWatch.hpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

fma: fma.o simd.o loop.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

%: %.cpp StopWatch.hpp Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
