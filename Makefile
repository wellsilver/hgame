all: build run clean

build:
	g++ src/main.cpp -o main.o -L lib -l glfw3 -l vulkan-1 -l gdi32

run:
	./main.o

clean:
	echo e
	rm main.o