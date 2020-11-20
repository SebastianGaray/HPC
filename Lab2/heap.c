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

int getFather (int actualPosition) {
	if (actualPosition == 0){
		return -1;
	}
	if (actualPosition % 2 == 0 ){
		return (actualPosition/2 -1);
	}
	return (actualPosition/2);
}

Heap *initHeap(){
    float *values = (float*)malloc(sizeof(float)*16);
    int lastPos = 0;
    Heap *heap = (Heap*)malloc(sizeof(Heap));
    heap->values = values;
    heap->lastPos = lastPos;
    return heap;
}

void *swap (Heap *h,int x, int y) {
    float aux  = h->values[x];
    h->values[x] = h->values[y];
    h->values[y] =aux;
}

void *heapifyDown(Heap *heap, int posActual){
    printf("Ordenando heap...\n");
    int fIndex = getFather(posActual);
    if (heap->values[posActual] < heap->values[fIndex]){
        swap(heap,posActual,fIndex);
        heapifyUp(heap, fIndex);
    }
}

void *heapifyUp(Heap *heap, int posActual){
    printf("Ordenando heap...\n");
    int fIndex = getFather(posActual);
    if (heap->values[posActual] < heap->values[fIndex]){
        swap(heap,posActual,fIndex);
        heapifyUp(heap, fIndex);
    }
}

void insertInHeap(Heap *heap, float value){
    printf("\nInsertando el valor: %f\n", value);
    heap->values[heap->lastPos] = value;
    heap->lastPos = heap->lastPos + 1;
    if(heap->lastPos != 1){
        heapifyUp(heap,heap->lastPos-1);
    }
}

float deleteFromHeap(Heap *heap){
    printf("\nEliminando la raiz del heap\n");
    float value = heap->values[0];
    heap->values[0] = heap->values[heap->lastPos-1];
    heap->values[heap->lastPos-1] = 0;
    heap->lastPos = heap->lastPos - 1;
        //heapifyDown(heap,0);
    return value;
}