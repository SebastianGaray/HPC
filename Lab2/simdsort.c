// C includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>    /* memcpy */
#include <cpuid.h>     /* __get_cpuid_max, __get_cpuid */
#include <mmintrin.h>  /* MMX instrinsics  __m64 integer type  */
#include <xmmintrin.h> /* SSE  __m128  float */




typedef struct myBMNResponse{
	__m128 s1;
	__m128 s2;
}BMNResponse;



void debug(__m128 vector){
	float arr[4] __attribute__ ((aligned (16))) = {0,0,0,0};
	_mm_store_ps(arr, vector);
	for(int i = 0; i<4; i++){
		printf("%f ",arr[i]);
	}
	printf("\n");
}

BMNResponse BMN(__m128 vector1, __m128 vector2){
	// 2 primeras filas
	__m128 minVector, maxVector;
	// Se invierte el 2do registro
	vector2 = _mm_shuffle_ps(vector2, vector2, _MM_SHUFFLE(0,1,2,3));
		
	// Primera etapa
	vector2			= _mm_shuffle_ps(vector2, vector2, _MM_SHUFFLE(3,1,2,0));
	vector1			= _mm_shuffle_ps(vector1, vector1, _MM_SHUFFLE(3,1,2,0));
	minVector 		= _mm_min_ps(vector1,vector2);
	maxVector 		= _mm_max_ps(vector1,vector2);
	// Segunda etapa
	__m128   Max1  	= _mm_shuffle_ps(minVector, maxVector, _MM_SHUFFLE(2,0,2,0));
	__m128   Min1  	= _mm_shuffle_ps(minVector, maxVector, _MM_SHUFFLE(3,1,3,1));

	Max1  			= _mm_shuffle_ps(Max1, Max1, _MM_SHUFFLE(2,0,3,1));
	Min1  			= _mm_shuffle_ps(Min1, Min1, _MM_SHUFFLE(2,0,3,1));
	
	minVector 		= _mm_min_ps(Max1,Min1);
	maxVector 		= _mm_max_ps(Max1,Min1);

	// Tercera etapa
	__m128   Max2  	= _mm_shuffle_ps(minVector, maxVector, _MM_SHUFFLE(1,0,1,0));
	__m128   Min2  	= _mm_shuffle_ps(minVector, maxVector, _MM_SHUFFLE(3,2,3,2));
	Max2  			= _mm_shuffle_ps(Max2, Max2, _MM_SHUFFLE(3,1,2,0));
	Min2  			= _mm_shuffle_ps(Min2, Min2, _MM_SHUFFLE(3,1,2,0));
	minVector 		= _mm_min_ps(Max2,Min2);
	maxVector 		= _mm_max_ps(Max2,Min2);

	__m128   s1 	=  _mm_shuffle_ps(minVector, maxVector, _MM_SHUFFLE(1,0,1,0));
	s1 				=  _mm_shuffle_ps(s1, s1, _MM_SHUFFLE(3,1,2,0));
	__m128   s2 	=  _mm_shuffle_ps(minVector, maxVector, _MM_SHUFFLE(3,2,3,2));
	s2				=  _mm_shuffle_ps(s2, s2, _MM_SHUFFLE(3,1,2,0));

	BMNResponse myResponse = {s1, s2};
	return myResponse;
}


void simdsort(int N, int debugFlag, float *data){
   	
	float *outputArray= (float*)malloc(sizeof(float)*N);
	// Se hace el proceso cada 16 datos
	for(int i = 0; i<N; i=i+16){
		__m128 reg1, reg2, reg3, reg4, maxVector, minVector;
		// Se divide la data en 4 arreglos
		float arr1[4] __attribute__ ((aligned (16))) = {data[i],data[i+1],data[i+2],data[i+3]};
		float arr2[4] __attribute__ ((aligned (16))) = {data[i+4],data[i+5],data[i+6],data[i+7]};
		float arr3[4] __attribute__ ((aligned (16))) = {data[i+8],data[i+9],data[i+10],data[i+11]};
		float arr4[4] __attribute__ ((aligned (16))) = {data[i+12],data[i+13],data[i+14],data[i+15]};
		// Se carga la data en los arreglos
		reg1 		= _mm_load_ps(arr1);
		reg2 		= _mm_load_ps(arr2);
		reg3 		= _mm_load_ps(arr3);
		reg4 		= _mm_load_ps(arr4);
		
		
		if(debugFlag == 1){
			printf("\nDivision 4 registros:\n");
			debug(reg1);
			debug(reg2);
			debug(reg3);
			debug(reg4);
		}
		//############################
		//#	Ordenamiento in register #
		//############################
		// Comparacion arreglo 1 con 3
		maxVector 	= _mm_max_ps(reg1,reg3);
		minVector 	= _mm_min_ps(reg1,reg3);
		// Se dejan los minimos en el registro "menor"
		reg1 		= minVector;
		reg3 		= maxVector;

		// [[min R1][maxR3]]
		// Comparacion arreglo 2 con 4
		maxVector 	= _mm_max_ps(reg2,reg4);
		minVector 	= _mm_min_ps(reg2,reg4);
		// Se dejan los minimos en el registro "menor"
		reg2 		= minVector;
		reg4 		= maxVector;
	

		// Comparacion arreglo 1 con 2 y 3 con 4
		maxVector 	= _mm_max_ps(reg1,reg2);
		minVector 	= _mm_min_ps(reg1,reg2);
		// Se dejan los minimos en el registro "menor"
		reg1 		= minVector;
		reg2 		= maxVector;
		maxVector 	= _mm_max_ps(reg3,reg4);
		minVector 	= _mm_min_ps(reg3,reg4);
		// Se dejan los minimos en el registro "menor"
		reg3 		= minVector;
		reg4 		= maxVector;

		 // Comparacion arreglo 2 con 3
		maxVector 	= _mm_max_ps(reg2,reg3);
		minVector 	= _mm_min_ps(reg2,reg3);
		// Se dejan los minimos en el registro "menor"
		reg2 		= minVector;
		reg3 		= maxVector;

		if(debugFlag == 1){
			printf("\nDespues de hacer min max:\n");
			debug(reg1);
			debug(reg2);
			debug(reg3);
			debug(reg4);
		}

		
		//////////////// 
		// TRANSPONER //
		////////////////
		__m128 A 	= _mm_shuffle_ps(reg1, reg3, _MM_SHUFFLE(1,0,1,0));
			   A 	= _mm_shuffle_ps(A,A,_MM_SHUFFLE(3,1,2,0));
	

		__m128 B 	= _mm_shuffle_ps(reg1, reg3, _MM_SHUFFLE(3,2,3,2));
			   B 	= _mm_shuffle_ps(B,B,_MM_SHUFFLE(3,1,2,0));
	
		__m128 C 	= _mm_shuffle_ps(reg2, reg4, _MM_SHUFFLE(1,0,1,0));
			   C 	= _mm_shuffle_ps(C,C,_MM_SHUFFLE(3,1,2,0));
	

		__m128 D 	= _mm_shuffle_ps(reg2, reg4, _MM_SHUFFLE(3,2,3,2));
			   D 	= _mm_shuffle_ps(D,D,_MM_SHUFFLE(3,1,2,0));


		__m128 A1 	= _mm_shuffle_ps(A, C, _MM_SHUFFLE(1,0,1,0));
			   A1 	= _mm_shuffle_ps(A1,A1,_MM_SHUFFLE(3,1,2,0));
	

		__m128 B1 	= _mm_shuffle_ps(A, C, _MM_SHUFFLE(3,2,3,2));
			   B1 	= _mm_shuffle_ps(B1,B1,_MM_SHUFFLE(3,1,2,0));
	
		__m128 C1 	= _mm_shuffle_ps(B, D, _MM_SHUFFLE(1,0,1,0));
			   C1 	= _mm_shuffle_ps(C1,C1,_MM_SHUFFLE(3,1,2,0));
	

		__m128 D1 	= _mm_shuffle_ps(B, D, _MM_SHUFFLE(3,2,3,2));
			   D1 	= _mm_shuffle_ps(D1,D1,_MM_SHUFFLE(3,1,2,0));
	
		if(debugFlag == 1){
			printf("\nDespues de transponer:\n");
			debug(A1);
			debug(B1);
			debug(C1);
			debug(D1);
		}
	 	
		/////////
		// BMN //
		/////////
		// 2 primeras filas
		BMNResponse firstBMNResponse = BMN(A1, B1);
		// 2 ultimas filas
		BMNResponse secondBMNResponse = BMN(D1, C1);
				

		if(debugFlag == 1){
			printf("\nDespues de hacer la primera BMN:\n");
			debug(firstBMNResponse.s1);
			debug(firstBMNResponse.s2);
			debug(secondBMNResponse.s1);
			debug(secondBMNResponse.s2);
		}
		
	
		//////////////////////////////
		// Merge SIMD de secuencias //
		//////////////////////////////

		// Se alimenta la BMN con los 4 primeros elementos de ambas secuencias
		BMNResponse firstMergeResponse = BMN(firstBMNResponse.s1, secondBMNResponse.s1);

		BMNResponse secondMergeResponse;
		// S2 pasa a ser S1, y para el 2do arreglo se utiliza aquel que tenga el menor elemento 0
		if(firstBMNResponse.s2[0] < secondBMNResponse.s2[0]){
			secondMergeResponse = BMN(firstMergeResponse.s2, firstBMNResponse.s2);
		} else {
			secondMergeResponse = BMN(firstMergeResponse.s2, secondBMNResponse.s2);
		}
		BMNResponse thirdMergeResponse;
		// S2 pasa a ser S1, y para el segundo arreglo se utiliza el que no se utilizo antes
		if(firstBMNResponse.s2[0] < secondBMNResponse.s2[0]){
			thirdMergeResponse = BMN(secondMergeResponse.s2, secondBMNResponse.s2);
		} else {
			thirdMergeResponse = BMN(secondMergeResponse.s2, firstBMNResponse.s2);
		}

		if(debugFlag == 1){
			printf("\nDespues de hacer Merge SIMD:\n");
			debug(firstMergeResponse.s1);
			debug(secondMergeResponse.s1);
			debug(thirdMergeResponse.s1);
			debug(thirdMergeResponse.s2);
		}
		// Se guardan los 4 registros en el arreglo final
		_mm_store_ps(arr1, firstMergeResponse.s1);
		_mm_store_ps(arr2, secondMergeResponse.s1);
		_mm_store_ps(arr3, thirdMergeResponse.s1);
		_mm_store_ps(arr4, thirdMergeResponse.s2);
		for(int j = 0; j<N/16; j++){
			outputArray[i+j] = arr1[j];
			outputArray[i+j+4] = arr2[j];
			outputArray[i+j+8] = arr3[j];
			outputArray[i+j+12] = arr4[j];
		}
	}
	if(debugFlag == 1){
		printf("\nArray final ordenado:\n");
		for(int x = 0; x<N; x++){
			printf("%f ",outputArray[x]);
		}
		printf("\n");
	}
	//sort(outputArray, N);
	
	
	//writeFile(outputFile, N, outputArray);
}
