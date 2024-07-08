all: build run clean

build:
	g++ src/main.cpp -o main.exe -g -municode -L lib -I lib -l OpenCL -l gdi32 -l d3d11

run:
	./main.exe

clean:
	echo e
	rm main.exe