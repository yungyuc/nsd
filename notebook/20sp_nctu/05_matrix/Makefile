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
	01_pod_vector 02_pod_matrix_auto 03_pod_matrix_rowmajor 04_pod_matrix_colmajor \
	05_matrix_class 06_matrix_vector 07_matrix_matrix \
	08_gesv 09_geev 10_syev 11_gesvd 12_gels

.PHONY: default
default: ${BINS}

%: %.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
