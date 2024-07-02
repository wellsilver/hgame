all: build run clean

build:
	g++ src/main.cpp -o main.o -g -municode -L lib -I lib -l OpenCL

run:
	./main.o

clean:
	echo e
	rm main.o