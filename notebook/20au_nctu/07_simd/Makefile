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
