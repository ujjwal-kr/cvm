./build/main: main.h add.c main.c mylibc/mem.h
	@mkdir -p ./build
	cc main.c add.c -o ./build/main

clean:
	@rm -rf ./build

run:
	@./build/main