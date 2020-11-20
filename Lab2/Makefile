CC=gcc
CFLAGS=-Wall

sort: sort.c simdsort.o files.o heap.o
	$(CC) $(CFLAGS) heap.o simdsort.o files.o sort.c -o sort

simdsort.o: simdsort.h simdsort.c
	$(CC) $(CFLAGS) simdsort.c -c

files.o: files.h files.c
	$(CC) $(CFLAGS) files.c -c

heap.o: heap.h heap.c
	$(CC) $(CFLAGS) heap.c -c



clean:
	rm *.o
	rm sort
	