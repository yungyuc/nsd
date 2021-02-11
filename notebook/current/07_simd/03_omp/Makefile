FLAGS ?=

UNAME_S := $(shell uname -s)

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

BINS := omp

.PHONY: default
default: ${BINS}

omp.o: omp.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

omp: omp.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

%: %.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
