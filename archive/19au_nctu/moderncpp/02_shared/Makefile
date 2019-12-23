FLAGS ?=

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -g -O3 -m64 \
	${FLAGS}

LINKFLAGS :=

BINS := \
	01_fully \
	02_duplicate \
	03_fromthis \
	04_cyclic \
	05_weak

.PHONY: default
default: ${BINS}

%.o: %.cpp Makefile
	${CXX} ${CXXFLAGS} -c -o $@ $<

%: %.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
