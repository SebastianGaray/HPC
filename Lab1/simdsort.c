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
	__m128i r0,r2,maxvector,arrvector;
    float arr[]  __attribute__ ((aligned (16))) = {8,4,9,1};
    float printvector[2] __attribute__ ((aligned (16)));
    int i;
	r0 = _mm_loadu_si128((__m128i*)&arr[0]);
	r2 = _mm_loadu_si128((__m128i*)&arr[1]);
	maxvector =  _mm_max_epi16(r0,r2);
    _mm_storeu_si128(printvector, maxvector);
    for (i = 0; i < 2; i++)
        printf("%f \n",  printvector[i]); 
	
}
