CXX:=g++
COMPILER_FLAGS:=-std=c++2a -Wall -Wextra -pedantic
LINKER_FLAGS:=`Magick++-config --cppflags --cxxflags --ldflags --libs`
target: all

all: image_gen

image_gen:
	$(CXX) $(COMPILER_FLAGS) -o image_gen main.cpp $(LINKER_FLAGS)

clean:
	rm -f image_gen