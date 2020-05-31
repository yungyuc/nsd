CC = gcc
CFLAGS = -std=c11 -O3 -g
CXX = g++
CXXFLAGS = -std=c++17 -O3 -g

.PHONY: default
default: cmem cppmem alloc icount mismatch

cmem.o: cmem.c Makefile
	${CC} ${CFLAGS} -o $@ -c $<

cmem: cmem.o Makefile
	${CC} ${CFLAGS} -o $@ $<

cppmem.o: cppmem.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

cppmem: cppmem.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

alloc.o: alloc.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

alloc: alloc.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

icount.o: icount.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

icount: icount.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

mismatch.o: mismatch.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

mismatch: mismatch.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -f *.o cmem cppmem alloc icount mismatch
