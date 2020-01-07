CXX = g++

hello: hello.o hellomain.o
	$(CXX) hello.o hellomain.o -o hello

hello.o: hello.cpp hello.hpp
	$(CXX) -c hello.cpp -o hello.o

hellomain.o: hellomain.cpp hello.hpp
	$(CXX) -c hellomain.cpp -o hellomain.o

# vim: set noet nobomb fenc=utf8 ff=unix:
