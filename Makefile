all: main.o
	gcc main.o -o bin
main.o: main.c
	gcc -I . -c main.c
clean:
	rm -rf *.o
	rm bin