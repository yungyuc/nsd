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
	-std=c++17 -O3 -g -m64 \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

BINS := \
	pod_vector pod_matrix_auto pod_matrix_rowmajor pod_matrix_colmajor \
	matrix_class matrix_vector matrix_matrix \
	gesv

.PHONY: default
default: ${BINS}

%: %.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
