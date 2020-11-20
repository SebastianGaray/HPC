#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "simdsort.h"
#include "files.h"
#include "heap.h"



int main(int argc, char **argv){
	int N;
    //int debugFlag = 0;
    //char *inputFile, *outputFile = NULL;
    char *inputFile;
	N = 64;
	inputFile = "64floats.raw";
	/* 
    while ((aux = getopt (argc, argv, ":i:o:N:d:")) != -1){
		switch (aux){
			case 'i':
				inputFile = optarg;
				break;
			case 'o':
				outputFile = optarg;
				break;
			case 'N':
				N = atoi(optarg);
				break;
			case 'd':
				debugFlag = atoi(optarg);
				break;
			case '?':
                fprintf(stderr, "ERROR, Opcion invalida: -%c\n", optopt);
                return EXIT_FAILURE;
			case ':':
                fprintf(stderr, "ERROR, Falta el argumento de la opcion: -%c\n", optopt);
                return EXIT_FAILURE;
		}
	}
	*/

	//float *data = readFile(inputFile, N);
	Heap *heap = initHeap();
	insertInHeap(heap, 7);
	insertInHeap(heap, 3);
	insertInHeap(heap, 5);
	insertInHeap(heap, 8);
	insertInHeap(heap, 1);
	insertInHeap(heap, 2);
    debugHeap(heap);

	return 1;
}

