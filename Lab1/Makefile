CC=gcc
CFLAGS=-Wall

simdsort: simdsortv2.c files.o
	$(CC) $(CFLAGS) simdsortv2.c -o simdsort

files.o: files.h files.c
	$(CC) $(CFLAGS) -c files.c -o files.o

clean:
	rm simdsort
	rm *.o