main: buffer.o main.o
		gcc -Wall -pedantic -o buffer buffer.o main.o
main.o: main.c buffer.h
		gcc -Wall -pedantic -c main.c
buffer.o: buffer.c buffer.h
		gcc -Wall -pedantic -c buffer.c
clean:
	rm -f main *.o