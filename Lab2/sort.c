#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <omp.h>
#include "simdsort.h"
#include "files.h"
#include <sys/times.h>



void multiway_sort(float *arrayNumbers,int length){
	Heap *h = initHeap(length/16);
	float outputArray[length];
	for ( int i = 0;i < length ;i +=16){
		insertInHeap(h, arrayNumbers[i],i);
	}
	for (int j = 0; j < length; j++){
		HeapValues x = deleteFromHeap(h);
		outputArray[j] = x.value;
		if ((x.originalListIndex+1)%16 != 0 && x.originalListIndex < length){
			insertInHeap(h, arrayNumbers[x.originalListIndex+1],x.originalListIndex+1);
		}
	}
	free(h->values);
	free(h);
	
	for(int i = 0; i<length; i++){
		arrayNumbers[i] = outputArray[i];
	}
	
}

void dividir(float *arrayNumbers, int length, int numLevels)
{
	if (numLevels == 0)
	{
		for ( int i = 0;i < length ;i +=16){
			 // debug, index, arreglo
			simdsort(0,i,arrayNumbers);
		}

		multiway_sort(arrayNumbers, length);
		return;
	}
	int mitad = length / 2;

#pragma omp task untied
	{
		dividir(arrayNumbers, mitad, numLevels - 1);
	}
#pragma omp task untied
	{
		dividir(arrayNumbers + mitad, mitad, numLevels - 1);
	}
	
}

void inicio(float *arrayNumbers, int length, int numThreads, int numLevels)
{
	omp_set_num_threads(numThreads);
	#pragma omp parallel
	dividir(arrayNumbers, length, numLevels);
	#pragma omp single nowait
	multiway_sort(arrayNumbers, length);
	/*
	for ( int i = 0;i < length ;i +=1){
		printf("%f ", arrayNumbers[i]);
	}
	*/
	printf("\n-----\n");
	return;
}

int main(int argc, char **argv)
{
	// N Hebras, l largo
	int numThreads, N, numLevels;
	char *inputFile, *outputFile,*outputFile2;
	numLevels = 2;
	inputFile = "65536floats.raw";
	outputFile = "salida.raw";
	outputFile2 = "salidaTEXT.txt";
	N = 65536;
	float arrayNumbers[N];
	readFile(inputFile, N, arrayNumbers);
	float speep[100];
	for(numThreads = 1; numThreads <= 50; numThreads++){
		printf("Comenzando configuracion:\n");
		printf("Hebras: {%i} size: {%i} niveles: {%i}\n", numThreads, N, numLevels);
		

		//Mido tiempo de inicio de procesamiento
		struct tms t_ini_struct;
		clock_t t_ini = times(&t_ini_struct);

		inicio(arrayNumbers, N, numThreads, numLevels);
		struct tms t_fin_struct;
		clock_t t_fin = times(&t_fin_struct);
		float s  = ( (t_fin - t_ini) / (double)sysconf(_SC_CLK_TCK) );
		printf("El algoritmo demoró %f segundos\n", s);
		speep[numThreads-1] = s;
	}


	FILE *file = fopen(outputFile2,"w");
	for(int i = 0; i<50; i++){
    	fwrite(&speep[i],sizeof(float),1,file);
	}
	fclose(file);
	//writeFile(outputFile, N, arrayNumbers);
	return 0;

	/*Heap *heap = initHeap();
	insertInHeap(heap, 88);
	insertInHeap(heap, 0);
	insertInHeap(heap, 28);
	insertInHeap(heap, 95);
	insertInHeap(heap, 92);
	insertInHeap(heap, 79);
	insertInHeap(heap, 0);
	insertInHeap(heap, 1);
	insertInHeap(heap, 92);
	insertInHeap(heap, 63);
	insertInHeap(heap, 13);
	
	float x;
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);
	x = deleteFromHeap(heap);


	//printf ("%f",x);
	debugHeap(heap);

	x = deleteFromHeap(heap);
	printf ("%f",x);
	x = deleteFromHeap(heap);
	printf ("%f",x);
	debugHeap(heap);
	//int N;
    //int debugFlag = 0;
    //char *inputFile, *outputFile = NULL;
    //char *inputFile;
	//N = 64;
	//inputFile = "64floats.raw";
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
	/*Heap *heap = initHeap();
	insertInHeap(heap, 7);
	insertInHeap(heap, 3);
	insertInHeap(heap, 5);
	insertInHeap(heap, 8);
	insertInHeap(heap, 1);
	insertInHeap(heap, 2);
    debugHeap(heap);*/

	return 1;
}
