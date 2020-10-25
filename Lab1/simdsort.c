// C includes
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

// User includes
#include "files.h"

int main(int argc, char **argv){
    int N, aux;
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
    printf("Debug flag: %i\n",debugFlag);
        
}