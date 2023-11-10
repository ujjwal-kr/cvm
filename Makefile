main: main.h add.c main.c
	cc main.c add.c -o ./build/main

run:
	@./build/main