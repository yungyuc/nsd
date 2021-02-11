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

BINS :=

.PHONY: default
default: ${BINS}
	make -C 01_mul
	make -C 02_fma
	make -C 03_omp

.PHONY: clean
clean:
	make -C 01_mul clean
	make -C 02_fma clean
	make -C 03_omp clean
	rm -rf *.o *.dSYM/ ${BINS}
