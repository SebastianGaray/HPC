#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <omp.h>
#include "simdsort.h"
#include "files.h"
#include <sys/times.h>



void inicio (float *arrayNumbers, int length, int numThreads){
 	int numLevels = 3;
	omp_set_num_threads(numThreads);
	#pragma omp parallel
	#pragma omp single nowait
	recorrer(arrayNumbers, length, numLevels);
}

void recorrer(float *arrayNumbers,int length, int numLevels){
	int tid = omp_get_thread_num();
	printf("thread %d\n", tid);
	for (int i = 0; i < length; ++i)
	{
		printf("%f-", arrayNumbers[i] );
	}
	printf("\n");
	 if(numLevels == 0){
		 int i;
		 for ( i = 0;i < length ;i +=16){
			Heap *h = simdsort(16,0, arrayNumbers);
			debugHeap(h);
		 }
		mostrar(arrayNumbers,length);
		return;
	 }
	 int mitad = length/2;
	
	#pragma omp task untied
	{
		recorrer(arrayNumbers, mitad, numLevels-1);
	}
	#pragma omp task untied
	{
		recorrer(arrayNumbers+mitad, mitad, numLevels-1);
	}

}
void suma(float *arrayNumbers,int length){

}
void mostrar(float *arrayNumbers,int length){

}


int main(int argc, char **argv){

	int N, l;
	N = 4; //hebras
	l = 32; //largo
	float arrayNumbers[l];
	for (int i = 0; i < l; ++i)
	{
		arrayNumbers[i] =  rand() % (l);
	}

	/*printf("los valores son: \n");
	for (int i = 0; i < l; ++i)
	{
		printf("%f \n", arrayNumbers[i] );
	}*/ 
	
	//Mido tiempo de inicio de procesamiento
	struct tms t_ini_struct;
	clock_t t_ini = times(&t_ini_struct);
	
	inicio(arrayNumbers, l, N);
	struct tms t_fin_struct;
	clock_t t_fin = times(&t_fin_struct);
	printf("El algoritmo demoró %.3f segundos\n", (t_fin-t_ini)/(double)sysconf(_SC_CLK_TCK));
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

	/*x = deleteFromHeap(heap);
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

