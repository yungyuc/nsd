FLAGS ?=
CC = gcc
CFLAGS = -std=c11 -O3 -g ${FLAGS}
CXX = g++
CXXFLAGS = -std=c++17 -O3 -g ${FLAGS}

.PHONY: default
default: pod_vector pod_matrix_auto pod_matrix_rowmajor pod_matrix_colmajor \
	matrix_class

pod_vector.o: pod_vector.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

pod_vector: pod_vector.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

pod_matrix_auto.o: pod_matrix_auto.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

pod_matrix_auto: pod_matrix_auto.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

pod_matrix_rowmajor.o: pod_matrix_rowmajor.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

pod_matrix_rowmajor: pod_matrix_rowmajor.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

pod_matrix_colmajor.o: pod_matrix_colmajor.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

pod_matrix_colmajor: pod_matrix_colmajor.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

pod_bad_matrix.o: pod_bad_matrix.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

pod_bad_matrix: pod_bad_matrix.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

matrix_class.o: matrix_class.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ -c $<

matrix_class: matrix_class.o Makefile
	${CXX} ${CXXFLAGS} -o $@ $<

.PHONY: clean
clean:
	rm -f *.o pod_vector \
		pod_matrix_auto pod_matrix_rowmajor pod_matrix_colmajor \
		matrix_class
