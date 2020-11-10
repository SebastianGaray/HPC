CC=gcc
CFLAGS=-Wall

simdsort: simdsort.c files.o sort.o
	$(CC) $(CFLAGS) sort.o files.o simdsort.c -o simdsort

files.o: files.h files.c
	$(CC) $(CFLAGS) -c files.c -o files.o

sort.o: sort.h sort.c
	$(CC) $(CFLAGS) -c sort.c -o sort.o


clean:
	rm *.o
	rm simdsort
	