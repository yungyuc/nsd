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
	-std=c++17 -g -O3 -m64 -mavx \
	${FLAGS}

LINKFLAGS := \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \

BINS := mul

.PHONY: default
default: ${BINS}

mul.o: mul.cpp StopWatch.hpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

mul: mul.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

%: %.cpp StopWatch.hpp Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

R2CMD := "e scr.color=0 ; \
s sym.multiply1_loop_float__float__float ; pdf ; \
s sym.multiply1_simd_float__float__float ; pdf ; \
s sym.multiply3_loop_float__float__float ; pdf ; \
s sym.multiply3_simd_float__float__float ; pdf ; \
s sym.multiply5_loop_float__float__float ; pdf ; \
s sym.multiply5_simd_float__float__float ; pdf"

.PHONY: r2
r2: Makefile
	r2 -Aqc ${R2CMD} mul

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
