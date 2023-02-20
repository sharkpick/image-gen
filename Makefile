CXX:=g++
COMPILER_FLAGS:=-std=c++2a -Wall -Wextra -pedantic
LINKER_FLAGS:=`Magick++-config --cppflags --cxxflags --ldflags --libs`
target: all

all: image_gen

image_gen:
	$(CXX) $(COMPILER_FLAGS) -o image_gen src/main.cpp $(LINKER_FLAGS)

image_gen_tests: tests/image_gen_tests.cpp
	$(CXX) $(COMPILER_FLAGS) -o image_gen_tests tests/image_gen_tests.cpp $(LINKER_FLAGS) -lgtest -lpthread

image_gen_benchmarks: tests/image_gen_benchmarks.cpp
	$(CXX) $(COMPILER_FLAGS) -o image_gen_benchmarks tests/image_gen_benchmarks.cpp $(LINKER_FLAGS) -lbenchmark -lpthread

clean:
	rm -f image_gen image_gen_tests image_gen_benchmarks