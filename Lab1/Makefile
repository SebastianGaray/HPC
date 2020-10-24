CC=gcc
CFLAGS=-Wall

simdsort: simdsort.c files.o
	$(CC) $(CFLAGS) simdsort.c -o simdsort

files.o: files.h files.c
	$(CC) $(CFLAGS) -c files.c -o files.o

clean:
	rm *.o