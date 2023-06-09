CXX=g++
CXXFLAGS=-W -Wall -Wextra -pedantic -ggdb3 -O0 -fsanitize=address,undefined  -pedantic -Wno-sign-conversion -Wno-sign-compare -ftrapv
build: main.cpp shader.h
	$(CXX) $(CXXFLAGS) -I./include -L./lib main.cpp -o main.bin -lglfw3 -lglad -lm

run: build
	./main.bin
clean:
	rm -rf main.bin
