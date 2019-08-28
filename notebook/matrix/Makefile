FLAGS ?=

MKLROOT ?= /opt/intel/mkl
MKLLINKLINE = ${MKLROOT}/lib/libmkl_intel_lp64.a ${MKLROOT}/lib/libmkl_sequential.a ${MKLROOT}/lib/libmkl_core.a -lpthread -lm -ldl
VECLIB_HEADER_PATH ?= /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers

CXX = g++
CXXFLAGS = \
	-std=c++17 -O3 -g -m64 \
	-I${VECLIB_HEADER_PATH} \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

BINS = \
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
