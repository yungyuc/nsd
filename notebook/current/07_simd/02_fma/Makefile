FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLINC ?= ${MKLROOT}/include
MKLLIB ?= ${MKLROOT}/lib
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLINC ?= /usr/include/mkl
MKLLIB ?= /usr/lib/x86_64-linux-gnu
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLLIB}/libmkl_intel_lp64.${MKLEXT} \
	${MKLLIB}/libmkl_sequential.${MKLEXT} \
	${MKLLIB}/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -g -O3 -m64 -mfma \
	${FLAGS}

LINKFLAGS := \
	-I${MKLINC} \
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
