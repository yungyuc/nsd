FLAGS ?=

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -g -O3 -m64 \
	${FLAGS}

LINKFLAGS :=

BINS := 01_raw_pointer 02_reference 03_ownership

.PHONY: default
default: ${BINS}

%.o: %.cpp Makefile
	${CXX} ${CXXFLAGS} -c -o $@ $<

%: %.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
