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
	fclose(file);
    return array;
}

void writeFile(char *outputFile, int N, float *outputArrayFinal){
    FILE *file = fopen(outputFile,"wb");
	for(int i = 0; i<N; i++){
    	fwrite(&outputArrayFinal[i],sizeof(float),1,file);
	}
	fclose(file);
}