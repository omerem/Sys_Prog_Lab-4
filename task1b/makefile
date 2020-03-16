
all: encoder


encoder: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o encoder


start.o: start.s
	nasm -g -f elf -w+all -o start.o start.s
main.o: main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o

util.o: util.c util.h
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o


.PHONY: clean

clean: 
	rm -f *.o encoder
	rm -f "text.txt"
