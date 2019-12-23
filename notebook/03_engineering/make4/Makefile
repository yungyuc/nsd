CXX = g++

# If the following two lines are commented out, the default target becomes hello.o.
.PHONY: default
default: hello

# Implicit rules will be skipped when searching for default.
#%.o: %.cpp hello.hpp
#	$(CXX) -c $< -o $@

hello.o: hello.cpp hello.hpp
	$(CXX) -c $< -o $@

hellomain.o: hellomain.cpp hello.hpp
	$(CXX) -c $< -o $@

hello: hello.o hellomain.o
	$(CXX) $^ -o $@

.PHONY: clean
clean:
	rm -rf hello *.o

# vim: set noet nobomb fenc=utf8 ff=unix:
