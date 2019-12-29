CXX = g++

hello: hello.o hellomain.o
	$(CXX) $^ -o $@

%.o: %.cpp hello.hpp
	$(CXX) -c $< -o $@

# vim: set noet nobomb fenc=utf8 ff=unix:
