// C includes
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>    /* memcpy */
#include <cpuid.h>     /* __get_cpuid_max, __get_cpuid */
#include <mmintrin.h>  /* MMX instrinsics  __m64 integer type  */
#include <xmmintrin.h> /* SSE  __m128  float */
// User includes
#include "files.h"

void printArr(float arr[4], int size){
	for(int i = 0; i<size; i++){
		printf("%f ",arr[i]);
	}
	printf("\n");
}


int main(int argc, char **argv){
   /* int N, aux;
    int debugFlag = 0;
    char *inputFile, *outputFile = NULL;
    while ((aux = getopt (argc, argv, "i:o:N:d:")) != -1){
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
			default:
				abort();
		}
	}
    printf("Input File: %s\n",inputFile);
    printf("Output File: %s\n",outputFile);
    printf("N: %i\n",N);
    printf("Debug flag: %i\n",debugFlag); */ 
	//float arr[4]  __attribute__ ((aligne(16))) = {8,4,9,1};
	/*__m128 A, B;
	//float a[4] __attribute__((aligne(16))) = {arr[(i*16)+0]}
	float arrAux  [4] __attribute__ ((aligne(16)));
	A = _mm_load_ps(arr);
	_mm_store_ps(arrAux,A);
	//B = _mm_max_ps(8,9); */  
	// X*y +32 

	/*
	__m128i r0,r2,maxvector,arrvector;
    float arr[]  __attribute__ ((aligned (16))) = {8,4,9,1};
    float printvector[2] __attribute__ ((aligned (16)));
    int i;
	r0 = _mm_loadu_si128((__m128i*)&arr[0]);
	r2 = _mm_loadu_si128((__m128i*)&arr[1]);
	maxvector =  _mm_max_epi16(r0,r2);
    _mm_storeu_si128(printvector, maxvector);
    */


	//float data[] __attribute__ ((aligned (16))) = {12,21,4,13,9,8,6,7,1,14,3,0,5,11,15,10};
	float data[] __attribute__ ((aligned (16))) = {12,21,4,13,9,8,6,7,1,14,3,0,5,11,15,10};
	//float data[] __attribute__ ((aligned (16))) = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

	int N = 16;
	// Se hace el proceso cada 16 datos
	for(int i = 0; i<N; i=i+16){
		__m128 reg1, reg2, reg3, reg4, maxVector, minVector, maxVector2, minVector2;
		// Se divide la data en 4 arreglos
		float arr1[4] __attribute__ ((aligned (16))) = {data[i],data[i+1],data[i+2],data[i+3]};
		float arr2[4] __attribute__ ((aligned (16))) = {data[i+4],data[i+5],data[i+6],data[i+7]};
		float arr3[4] __attribute__ ((aligned (16))) = {data[i+8],data[i+9],data[i+10],data[i+11]};
		float arr4[4] __attribute__ ((aligned (16))) = {data[i+12],data[i+13],data[i+14],data[i+15]};
		float printvector[4] __attribute__ ((aligned (16)));
		printf("Original: \n");
		printArr(arr1, 4);
		printArr(arr2, 4);
		printArr(arr3, 4);
		printArr(arr4, 4);
		// Se carga la data en los arreglos
		reg1 = _mm_load_ps(arr1);
		reg2 = _mm_load_ps(arr2);
		reg3 = _mm_load_ps(arr3);
		reg4 = _mm_load_ps(arr4);

		//############################
		//#	Ordenamiento in register #
		//############################
		// Comparacion arreglo 1 con 3
		maxVector = _mm_max_ps(reg1,reg3);
		minVector = _mm_min_ps(reg1,reg3);
		// Se dejan los minimos en el registro "menor"
		reg1 = minVector;
		reg3 = maxVector;

		// [[min R1][maxR3]]
		// Comparacion arreglo 2 con 4
		maxVector = _mm_max_ps(reg2,reg4);
		minVector = _mm_min_ps(reg2,reg4);
		// Se dejan los minimos en el registro "menor"
		reg2 = minVector;
		reg4 = maxVector;
	

		// Comparacion arreglo 1 con 2 y 3 con 4
		maxVector = _mm_max_ps(reg1,reg2);
		minVector = _mm_min_ps(reg1,reg2);
		// Se dejan los minimos en el registro "menor"
		reg1 = minVector;
		reg2 = maxVector;
		maxVector = _mm_max_ps(reg3,reg4);
		minVector = _mm_min_ps(reg3,reg4);
		// Se dejan los minimos en el registro "menor"
		reg3 = minVector;
		reg4 = maxVector;

		 // Comparacion arreglo 2 con 3
		maxVector = _mm_max_ps(reg2,reg3);
		minVector = _mm_min_ps(reg2,reg3);
		// Se dejan los minimos en el registro "menor"
		reg2 = minVector;
		reg3 = maxVector;



		printf("\nDespues de min max:\n");
		 _mm_store_ps(arr1, reg1);
		 _mm_store_ps(arr2, reg2);
		 _mm_store_ps(arr3, reg3);
		 _mm_store_ps(arr4, reg4);
		printArr(arr1, 4);
		printArr(arr2, 4);
		printArr(arr3, 4);
		printArr(arr4, 4);

		__m128 A = _mm_shuffle_ps(reg1, reg3, _MM_SHUFFLE(1,0,1,0));
			   A = _mm_shuffle_ps(A,A,_MM_SHUFFLE(3,1,2,0));
	

		__m128 B = _mm_shuffle_ps(reg1, reg3, _MM_SHUFFLE(3,2,3,2));
			   B = _mm_shuffle_ps(B,B,_MM_SHUFFLE(3,1,2,0));
	
		__m128 C = _mm_shuffle_ps(reg2, reg4, _MM_SHUFFLE(1,0,1,0));
			   C = _mm_shuffle_ps(C,C,_MM_SHUFFLE(3,1,2,0));
	

		__m128 D = _mm_shuffle_ps(reg2, reg4, _MM_SHUFFLE(3,2,3,2));
			   D = _mm_shuffle_ps(D,D,_MM_SHUFFLE(3,1,2,0));


		__m128 A1 = _mm_shuffle_ps(A, C, _MM_SHUFFLE(1,0,1,0));
			   A1 = _mm_shuffle_ps(A1,A1,_MM_SHUFFLE(3,1,2,0));
	

		__m128 B1 = _mm_shuffle_ps(A, C, _MM_SHUFFLE(3,2,3,2));
			   B1 = _mm_shuffle_ps(B1,B1,_MM_SHUFFLE(3,1,2,0));
	
		__m128 C1 = _mm_shuffle_ps(B, D, _MM_SHUFFLE(1,0,1,0));
			   C1 = _mm_shuffle_ps(C1,C1,_MM_SHUFFLE(3,1,2,0));
	

		__m128 D1 = _mm_shuffle_ps(B, D, _MM_SHUFFLE(3,2,3,2));
			   D1 = _mm_shuffle_ps(D1,D1,_MM_SHUFFLE(3,1,2,0));
	
	 	printf("\nDespues de transponer:\n");
		 _mm_store_ps(arr1, A1);
		 _mm_store_ps(arr2, B1);
		 _mm_store_ps(arr3, C1);
		 _mm_store_ps(arr4, D1);
		printArr(arr1, 4);
		printArr(arr2, 4);
		printArr(arr3, 4);
		printArr(arr4, 4);

		// => A1 B1
		// => 

	
		/////////////
		// SHUFFLE //
		/////////////
		// 2 primeras filas
		// Se invierte el 2do registro
		B1 = _mm_shuffle_ps(B1, B1, _MM_SHUFFLE(0,1,2,3));
		
		// Primera etapa
		__m128  B2= _mm_shuffle_ps(B1, B1, _MM_SHUFFLE(3,1,2,0));
		__m128  A2= _mm_shuffle_ps(A1, A1, _MM_SHUFFLE(3,1,2,0));
		minVector = _mm_max_ps(A2,B2);
		maxVector = _mm_min_ps(A2,B2);

		// Segunda etapa
		__m128   Max1  = _mm_shuffle_ps(maxVector, minVector, _MM_SHUFFLE(2,0,2,0));
		__m128   Min1  = _mm_shuffle_ps(maxVector, minVector, _MM_SHUFFLE(3,1,3,1));
		Max1  = _mm_shuffle_ps(Max1, Max1, _MM_SHUFFLE(2,0,3,1));
		Min1  = _mm_shuffle_ps(Min1, Min1, _MM_SHUFFLE(2,0,3,1));
		minVector = _mm_max_ps(Max1,Min1);
		maxVector = _mm_min_ps(Max1,Min1);

		// Tercera etapa
		__m128   Max2  = _mm_shuffle_ps(maxVector, minVector, _MM_SHUFFLE(1,0,1,0));
		__m128   Min2  = _mm_shuffle_ps(maxVector, minVector, _MM_SHUFFLE(3,2,3,2));
		Max2  = _mm_shuffle_ps(Max2, Max2, _MM_SHUFFLE(3,1,2,0));
		Min2  = _mm_shuffle_ps(Min2, Min2, _MM_SHUFFLE(3,1,2,0));
		minVector2 = _mm_max_ps(Max2,Min2);
		maxVector2 = _mm_min_ps(Max2,Min2);
		
		// 2 ultimas filas
		// Se invierte el 2do registro
		D1 = _mm_shuffle_ps(D1, D1, _MM_SHUFFLE(0,1,2,3));

		// Primera etapa
		__m128  D2= _mm_shuffle_ps(D1, D1, _MM_SHUFFLE(3,1,2,0));
		__m128  C2= _mm_shuffle_ps(C1, C1, _MM_SHUFFLE(3,1,2,0));
		minVector2 = _mm_max_ps(C2,D2);
		maxVector2 = _mm_min_ps(C2,D2);

		// Segunda etapa
		__m128   Max3  = _mm_shuffle_ps(maxVector2, minVector2, _MM_SHUFFLE(2,0,2,0));
		__m128   Min3  = _mm_shuffle_ps(maxVector2, minVector2, _MM_SHUFFLE(3,1,3,1));
		Max3  = _mm_shuffle_ps(Max3, Max3, _MM_SHUFFLE(2,0,3,1));
		Min3  = _mm_shuffle_ps(Min3, Min3, _MM_SHUFFLE(2,0,3,1));
		minVector2 = _mm_max_ps(Max3,Min3);
		maxVector2 = _mm_min_ps(Max3,Min3);

		// Tercera etapa
		__m128   Max4  = _mm_shuffle_ps(maxVector2, minVector2, _MM_SHUFFLE(1,0,1,0));
		__m128   Min4  = _mm_shuffle_ps(maxVector2, minVector2, _MM_SHUFFLE(3,2,3,2));
		Max4  = _mm_shuffle_ps(Max4, Max4, _MM_SHUFFLE(3,1,2,0));
		Min4  = _mm_shuffle_ps(Min4, Min4, _MM_SHUFFLE(3,1,2,0));
		minVector2 = _mm_max_ps(Max4,Min4);
		maxVector2 = _mm_min_ps(Max4,Min4);

		


		printf("\n\n\n");
		_mm_store_ps(arr1, maxVector);
		_mm_store_ps(arr2, minVector);
		printArr(arr1, 4);
		printArr(arr2, 4);
		_mm_store_ps(arr1, maxVector2);
		_mm_store_ps(arr2, minVector2);
		printArr(arr1, 4);
		printArr(arr2, 4);




			


		/*printf("Despues de min max:\n");
		 _mm_store_ps(arr1, maxVector);
		 _mm_store_ps(arr2, minVector);

		printArr(arr1, 4);
		printArr(arr2, 4);
 */ 

	/*
	__m128 A, B;
	float arr1[]  __attribute__ ((aligned (16))) = {8,4,9,1};
	float arrAux  [4] __attribute__ ((aligne(16)));
	A = _mm_load_ps(arr);
	_mm_store_ps(arrAux,A);
	for (i = 0; i < 2; i++)
        printf("%f \n",  printvector[i]); 
	*/
	}
}
