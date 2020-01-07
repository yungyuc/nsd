FLAGS ?=

OPT ?= -O3

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -g ${OPT} -m64 \
	${FLAGS}

LINKFLAGS :=

BINS := \
	01_lambda \
	02_stored \
	03_closure

.PHONY: default
default: ${BINS}

%.o: %.cpp Makefile
	${CXX} ${CXXFLAGS} -c -o $@ $<

%: %.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
