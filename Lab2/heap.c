#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

void debugHeap(Heap *heap){
    printf("\nLast pos: %i\n", heap->lastPos);
    printf("Values:\n");
	for(int i = 0; i<16; i++){
		printf("%f ",heap->values[i]);
	}
	printf("\n");
}

Heap *initHeap(){
    float *values = (float*)malloc(sizeof(float)*16);
    int lastPos = 0;
    Heap *heap = (Heap*)malloc(sizeof(Heap));
    heap->values = values;
    heap->lastPos = lastPos;
    return heap;
}

Heap *heapifyUp(Heap *heap){
    printf("Ordenando heap...\n");
    // Se revisa cada una de las posibilidades de que el hijo sea mayor que el padre
    float swap;
    if(heap->lastPos == 2 && heap->values[heap->lastPos-1] < heap->values[heap->lastPos-2]){
        swap = heap->values[heap->lastPos-2];
        heap->values[heap->lastPos-2] = heap->values[heap->lastPos-1];
        heap->values[heap->lastPos-1] = swap;
    }
    if((heap->lastPos == 3 || heap->lastPos == 4) && heap->values[heap->lastPos-1] < heap->values[heap->lastPos-3]){
        swap = heap->values[heap->lastPos-3];
        heap->values[heap->lastPos-3] = heap->values[heap->lastPos-1];
        heap->values[heap->lastPos-1] = swap;
    }
    if((heap->lastPos == 5 || heap->lastPos == 6) && heap->values[heap->lastPos-1] < heap->values[heap->lastPos-4]){
        swap = heap->values[heap->lastPos-4];
        heap->values[heap->lastPos-4] = heap->values[heap->lastPos-1];
        heap->values[heap->lastPos-1] = swap;
    }
}

void insertInHeap(Heap *heap, float value){
    printf("\nInsertando el valor: %f\n", value);
    heap->values[heap->lastPos] = value;
    heap->lastPos = heap->lastPos + 1;
    if(heap->lastPos != 1){
        heapifyUp(heap);
    }
}