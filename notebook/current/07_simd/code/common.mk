FLAGS ?=

UNAME_S := $(shell uname -s)

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -g -O3 -m64 -mavx \
	${FLAGS}

R2SETUP ?= e scr.color=0 ;
