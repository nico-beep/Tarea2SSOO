CC=gcc
LINKS=-lpthread -lrt

all: main

main: main.o
	$(CC) main.o -o main

main.o: main.c
	$(CC) -c -std=gnu99 main.c 

run: main
	$ ./main

clean:
	-rm -f *.o main
