include ../common.mk

BIN := mul

.PHONY: default
default: ${BIN}

mul.o: mul.cpp StopWatch.hpp Makefile
	${CXX} ${CXXFLAGS} -c -o $@ $<

mul: mul.o
	${CXX} ${CXXFLAGS} ${LINKFLAGS} -o $@ $^

.PHONY: run
run: ${BIN}
	./${BIN}

r2sym: ${BIN}
	r2 -Aqc "${R2SETUP} afl" ${BIN}

R2FHEAD ?= sym.
R2FTAIL ?= _float__float__float_

r2: ${BIN}
ifndef NAME
	@echo "variable NAME is not defined, exit"
else
	$(eval R2FNAME := ${R2FHEAD}${NAME}${R2FTAIL})
	r2 -Aqc "${R2SETUP} sf ${R2FNAME} ; pdf" ${BIN}
endif

r2all: ${BIN}
	r2 -Aqc "${R2SETUP} \
		sf ${R2FHEAD}multiply1_loop${R2FTAIL} ; pdf ; \
		sf ${R2FHEAD}multiply1_simd${R2FTAIL} ; pdf ; \
		sf ${R2FHEAD}multiply3_loop${R2FTAIL} ; pdf ; \
		sf ${R2FHEAD}multiply3_simd${R2FTAIL} ; pdf ; \
		sf ${R2FHEAD}multiply5_loop${R2FTAIL} ; pdf ; \
		sf ${R2FHEAD}multiply5_simd${R2FTAIL} ; pdf" \
		${BIN}

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BIN}
