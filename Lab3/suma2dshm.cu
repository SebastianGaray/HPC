#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <omp.h>
#include <sys/times.h>
#include <time.h>
#include <sys/times.h>
#include <stdio.h>
const int threadsPerBlock = 16;

// Funcion que calcula la suma de vecindades en cpu
void suma2d_cpu(float *A, float *B, int N, int V){
  for(int posX = 0; posX < N; posX++){
    for(int posY = 0; posY < N; posY++){
      for(int i = (posX-V); i<=(posX+V); i++){
        for(int j = (posY-V); j<=(posY+V); j++){
          if(!(i < 0 || i >= N || j < 0 || j >= N)){
            B[posX*N+posY] = B[posX*N+posY] + A[i*N+j];
          }
        }
      } 
    } 
  }
}



// Memoria compartida
__global__ void suma2dshm(float *A, float *B, int N, int V)
{
	// Se define la variable compartida temp, con el tamaño del bloque
  __shared__ float temp[threadsPerBlock*threadsPerBlock];
  // Se obtiene la pos x de la hebra
	int posX = blockDim.x*blockIdx.x + threadIdx.x;
 
	// Se obtiene la pos y de la hebra
	int posY = blockDim.y*blockIdx.y + threadIdx.y; 

	for(int i = (posX-V); i<=(posX+V); i++){
		for(int j = (posY-V); j<=(posY+V); j++){
			if(!(i < 0 || i >= N || j < 0 || j >= N)){
        temp[threadIdx.x*threadsPerBlock+threadIdx.y] = temp[threadIdx.x*threadsPerBlock+threadIdx.y] + A[i*N+j];
			}
		}
	}
	
	B[posX*N+posY] = temp[threadIdx.x*threadsPerBlock+threadIdx.y];	
}

int main(int argc, char **argv){
	int N, V, Bs, aux;;
    while ((aux = getopt (argc, argv, ":N:B:V:")) != -1){
		switch (aux){
			case 'N':
				N = atoi(optarg);
				break;
            case 'V':
				V = atoi(optarg);
				break;
			case 'B':
				Bs = atoi(optarg);
				break;
			case '?':
                fprintf(stderr, "ERROR, Opcion invalida: -%c\n", optopt);
                return EXIT_FAILURE;
			case ':':
                fprintf(stderr, "ERROR, Falta el argumento de la opcion: -%c\n", optopt);
                return EXIT_FAILURE;
		}
    }
    int size = N*N*sizeof(float);
	srand(time(NULL)); 
    //MEMORIA HOST
	float *A = (float *)malloc(size);
    float *B = (float *)malloc(size);
    // SE INICIALIZA
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            A[i*N+j] = (float)rand()/RAND_MAX;
            B[i*N+j] = 0;
        }
    }
	//COPIA DESDE MEMORIA A DEVICE
    float *d_a;
    float *d_b;
	cudaMalloc((void **) &d_a, size);
	cudaMalloc((void **) &d_b, size);
    cudaError err = cudaMemcpy(d_a, A, size, cudaMemcpyHostToDevice);
    if(err!=cudaSuccess) {
        printf("Error al copiar hacia device arreglo A: %s\n", cudaGetErrorString(err));
    }
    err = cudaMemcpy(d_b, B, size, cudaMemcpyHostToDevice);
    if(err!=cudaSuccess) {
        printf("Error al copiar hacia device arreglo B: %s\n", cudaGetErrorString(err));
    }
    
    //TAMAÑO BLOQUES
    dim3 gridSize = dim3(N/Bs, N/Bs);
    dim3 blockSize = dim3(Bs, Bs);
    
	//LLAMADO A KERNEL
    // Nota: Se comenta la medicion del tiempo del programa
    //cudaEvent_t start, stop;
    //float elapsedTime = 0;
    //cudaEventCreate(&start);
    //cudaEventCreate(&stop);
    //cudaEventRecord(start, 0);
	suma2dshm<<<gridSize, blockSize>>>(d_a, d_b, N, V);
    //cudaEventRecord(stop, 0);
    //cudaEventSynchronize(stop);
    //cudaEventElapsedTime(&elapsedTime, start, stop);
	//printf("El tiempo de ejecucion en GPU fue de: %f segundos\n", elapsedTime/1000);
    //COPIA RESULTADO DESDE DEVICE A HOST
    err = cudaMemcpy(B, d_b, size, cudaMemcpyDeviceToHost);
    if(err!=cudaSuccess) {
        printf("Error al copiar hacia host arreglo B: %s\n", cudaGetErrorString(err));
    }
    printf("Resultado CUDA:\n");
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            printf("%f ", B[i*N+j]);
        }
        printf("\n");
    }
	cudaFree(d_a);
    cudaFree(d_b);
     
    ///////
    //CPU//
    ///////

    float *B_GPU = (float *)malloc(size);
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            B_GPU[i*N+j] = 0;
        }
    }
    // Nota: Se comenta la medicion del tiempo del programa
    // clock_t t; 
    // t = clock(); 
    suma2d_cpu(A, B_GPU, N,V);
    //t = clock() - t; 
    //double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
	//printf("El tiempo de ejecucion en CPU fue de: %f segundos\n", time_taken);
    printf("Resultado CPU:\n");
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            printf("%f ", B_GPU[i*N+j]);
        }
        printf("\n");
    }
    free(A);
    free(B);
    free(B_GPU);
    return 0;

}
