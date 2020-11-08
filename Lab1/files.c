// C includes
#include <stdlib.h>
#include <stdio.h>

// User includes
#include "files.h"


float *readFile(char *inputFile, int N){
    FILE *file = fopen(inputFile,"rb");
    float *array= (float*)malloc(sizeof(float)*N);
	if (file == NULL)
	{
		fprintf(stderr, "ERROR, Archivo de entrada no encontrado\n");

		exit(EXIT_FAILURE);
	}
    fread(array, sizeof(float), N, file);

    return array;
}
