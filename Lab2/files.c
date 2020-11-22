// C includes
#include <stdlib.h>
#include <stdio.h>

// User includes
#include "files.h"


void readFile(char *inputFile, int N, float *array){
    FILE *file = fopen(inputFile,"rb");
	if (file == NULL)
	{
		fprintf(stderr, "ERROR, Archivo de entrada no encontrado\n");

		exit(EXIT_FAILURE);
	}
    fread(array, sizeof(float), N, file);
	fclose(file);
}

void writeFile(char *outputFile, int N, float *outputArrayFinal){
    FILE *file = fopen(outputFile,"wb");
	for(int i = 0; i<N; i++){
    	fwrite(&outputArrayFinal[i],sizeof(float),1,file);
	}
	fclose(file);
}