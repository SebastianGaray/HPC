#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <omp.h>
#include "simdsort.h"
#include "files.h"
#include <sys/times.h>


/*
Funcion que realiza el multiwaySort, haciendo uso de un heap.
Entrada: El arreglo de datos flotantes, y el largo de este.
Salida: Se modifica el arreglo de flotantes al ser un paso por referencia
*/
void multiwaySort(float *data,int length){
	Heap *h = initHeap(length/16);
	float outputArray[length];
	// Se agregan los primeros datos de las N listas, donde al ser un arreglo,
	// estas estan cada 16 datos
	for ( int i = 0;i < length ;i +=16){
		insertInHeap(h, data[i],i);
	}
	// Hasta que no queden mas datos que recorrer, se elimina del heap (eliminando el menor),
	// y se inserta el valor siguiente de la lista desde donde se saco
	for (int j = 0; j < length; j++){
		HeapValues x = deleteFromHeap(h);
		outputArray[j] = x.value;
		if ((x.originalListIndex+1)%16 != 0 && x.originalListIndex < length){
			insertInHeap(h, data[x.originalListIndex+1],x.originalListIndex+1);
		}
	}
	// Se libera el heap
	free(h->values);
	free(h);
	// Se copia hacia el arreglo original
	for(int i = 0; i<length; i++){
		data[i] = outputArray[i];
	}
	
}

/*
Funcion que divide a traves de recursion en los distintos niveles y
asignando distintas tareas de OpenMP. Luego, llama tanto al simdsort cada 16 datos,
como al multiwaySort.
Entrada: El arreglo de datos flotantes, la cantidad de niveles, y el largo de este.
Salida: Se modifica el arreglo de flotantes al ser un paso por referencia
*/
void split(float *data, int length, int numLevels){
	if (numLevels == 0){
		for ( int i = 0;i < length ;i +=16){
			simdsort(i,data);
		}
		multiwaySort(data, length);
		return;
	}
	int half = length / 2;
	#pragma omp task untied
	{
		split(data, half, numLevels - 1);
	}
	#pragma omp task untied
	{
		split(data + half, half,  numLevels - 1);
	}
	
}

/*
Funcion que comienza la ejecucion de las hebras, y espera a que terminen para hacer el merge
Entrada: El arreglo de datos flotantes, el largo de este, la cantidad de hebras, y la cantidad de niveles.
Salida: Se modifica el arreglo de flotantes al ser un paso por referencia
*/
void start(float *data, int length, int numThreads, int numLevels){
	omp_set_num_threads(numThreads);
	#pragma omp parallel
	#pragma omp single nowait
	split(data, length, numLevels);
	// Se realiza el merge, y puesto que es el mismo proceso que se hace en el multiwaySort,
	// se utiliza este para realizarlo
	// Ya que se esperaron a las demas hebras, el que comenzo a dividir es el encargado de hacer el merge
	// MERGE
	multiwaySort(data, length); // MERGE
	// MERGE
	return;
}



int main(int argc, char **argv){
	int N, aux, numThreads, numLevels;
    int debugFlag = 0;
    char *inputFile, *outputFile = NULL;
    while ((aux = getopt (argc, argv, ":i:o:N:d:l:h:")) != -1){
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
			case 'l':
				numLevels = atoi(optarg);
				break;
			case 'h':
				numThreads = atoi(optarg);
				break;
			case '?':
                fprintf(stderr, "ERROR, Opcion invalida: -%c\n", optopt);
                return EXIT_FAILURE;
			case ':':
                fprintf(stderr, "ERROR, Falta el argumento de la opcion: -%c\n", optopt);
                return EXIT_FAILURE;
		}
	}
	// Comprobacion de que las divisiones seran mayores a 16 para el simd
	if(numLevels != 0 && N/(numLevels*2) < 16){
		printf("Error, la cantidad de datos que entrarian al SIMD serian menor de 16, por lo que se dentra la ejecucion.\nPruebe con otra configuracion\n");
		return EXIT_FAILURE;
	}
	float data[N];
	// Se lee el archivo de entrada
	readFile(inputFile, N, data);

	// Nota: Se comenta la medicion del tiempo del programa
	//struct tms t_ini_struct;
	//clock_t t_ini = times(&t_ini_struct);
	start(data, N, numThreads, numLevels);
	//struct tms t_fin_struct;
	//clock_t t_fin = times(&t_fin_struct);
	//float s  = ( (t_fin - t_ini) / (double)sysconf(_SC_CLK_TCK) );
	//printf("El tiempo de ejecucion fue de: %f segundos\n", s);
	if(debugFlag == 1){
		for(int i = 0; i<N; i++){
			printf("%f\n",data[i]);
		}
	}		
	writeFile(outputFile, N, data);
	return 0;

}
