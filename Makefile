CXX:=g++
COMPILER_FLAGS:=-std=c++2a -Wall -Wextra -pedantic
LINKER_FLAGS:=`Magick++-config --cppflags --cxxflags --ldflags --libs`
VERSION:=0.0.1
ARCHITECTURE:=amd64
EXECUTABLE_NAME:=image-gen
DEB_NAME:=$(EXECUTABLE_NAME)_$(VERSION)_$(ARCHITECTURE)

target: all

all: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME):
	$(CXX) $(COMPILER_FLAGS) -o $(EXECUTABLE_NAME) src/main.cpp $(LINKER_FLAGS)

image_gen_tests: tests/image_gen_tests.cpp
	$(CXX) $(COMPILER_FLAGS) -o image_gen_tests tests/image_gen_tests.cpp $(LINKER_FLAGS) -lgtest -lpthread

image_gen_benchmarks: tests/image_gen_benchmarks.cpp
	$(CXX) $(COMPILER_FLAGS) -o image_gen_benchmarks tests/image_gen_benchmarks.cpp $(LINKER_FLAGS) -lbenchmark -lpthread

deb:
	mkdir -p $(EXECUTABLE_NAME)/$(EXECUTABLE_NAME)-extras $(EXECUTABLE_NAME)/${EXECUTABLE_NAME}-licenses $(DEB_NAME)/opt/
	$(CXX) $(COMPILER_FLAGS) -o $(EXECUTABLE_NAME)/$(EXECUTABLE_NAME) src/main.cpp $(LINKER_FLAGS)
	cp LICENSE $(EXECUTABLE_NAME)/$(EXECUTABLE_NAME)-licenses
	cp README.md $(EXECUTABLE_NAME)/$(EXECUTABLE_NAME)-extras
	cp -r DEBIAN $(DEB_NAME)
	mv $(EXECUTABLE_NAME) $(DEB_NAME)/opt
	dpkg-deb --build --root-owner-group $(DEB_NAME)	

clean:
	rm -rf $(EXECUTABLE_NAME) image_gen_tests image_gen_benchmarks $(EXECUTABLE_NAME)_*_$(ARCHITECTURE)*