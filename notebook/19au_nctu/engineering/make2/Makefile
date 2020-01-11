CXX = g++

hello: hello.o hellomain.o
	$(CXX) $^ -o $@

hello.o: hello.cpp hello.hpp
	$(CXX) -c $< -o $@

hellomain.o: hellomain.cpp hello.hpp
	$(CXX) -c $< -o $@

# vim: set noet nobomb fenc=utf8 ff=unix:
