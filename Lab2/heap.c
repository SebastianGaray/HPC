#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

void debugHeap(Heap *heap){
    printf("\nLast pos: %i\n", heap->lastPos);
    printf("Values:\n");
	for(int i = 0; i<heap->size; i++){
		printf("valor: %f ",heap->values[i].value);
        printf("viene de: %d ",heap->values[i].originalListIndex);
	    printf("\n");

	}
	printf("\n");
}

int getFather (int actualPosition) {
	if (actualPosition == 0){
		return -2;
	}
	if (actualPosition % 2 == 0 ){
		return (actualPosition/2 -1);
	}
	return (actualPosition/2);
}
int getLeft (int actualPosition) {
	return (2*actualPosition +1);
}
int getRight (int actualPosition) {
	return (2*actualPosition +2);
}


Heap *initHeap(int size){
    HeapValues *values = (HeapValues*)malloc(sizeof(HeapValues)*size);
    int lastPos = 0;
    Heap *heap = (Heap*)malloc(sizeof(Heap));
    heap->values = values;
    heap->lastPos = lastPos;
    heap->size = size;
    return heap;
}

void swap (Heap *h,int x, int y) {
    
    float aux  = h->values[x].value;
    int aux2  = h->values[x].originalListIndex;
    
    h->values[x].value = h->values[y].value;
    h->values[x].originalListIndex = h->values[y].originalListIndex;

    h->values[y].value =aux;
    h->values[y].originalListIndex =aux2;

}

void heapifyDown(Heap *heap, int posActual){
    int sLeftIndex = getLeft(posActual);
    int sRightIndex = getRight(posActual);
    float min;
    int minIndex;
    /* Caso 1 : */ 
    /*  No tengo hijos */
    // Mayor o igual ?
    if  ( (sLeftIndex  >= heap->lastPos) &&  (sRightIndex >= heap->lastPos)){
        return;
    }
    else if (sLeftIndex  >= heap->lastPos){
        min =  heap->values[sRightIndex].value;
        minIndex = sRightIndex;
    }
    else if (sRightIndex >=  heap->lastPos){
        min =  heap->values[sLeftIndex].value;
        minIndex = sLeftIndex;
    }
    else {
        min = (heap->values[sLeftIndex].value < heap->values[sRightIndex].value ? heap->values[sLeftIndex].value: heap->values[sRightIndex].value);
        minIndex =  (heap->values[sLeftIndex].value < heap->values[sRightIndex].value ? sLeftIndex: sRightIndex);
    }
    if (heap->values[posActual].value > min){
         swap(heap,posActual,minIndex);
         heapifyDown(heap,minIndex);
    }
}

void heapifyUp(Heap *heap, int posActual){
    int fIndex = getFather(posActual);
    if (heap->values[posActual].value < heap->values[fIndex].value){
        swap(heap,posActual,fIndex);
        heapifyUp(heap, fIndex);
    }
}

void insertInHeap(Heap *heap, float value, int originalListIndex){
    heap->values[heap->lastPos].value = value;
    heap->values[heap->lastPos].originalListIndex = originalListIndex;
    heap->lastPos = heap->lastPos + 1;
    if(heap->lastPos != 1){
        heapifyUp(heap,heap->lastPos-1);
    }
}

HeapValues deleteFromHeap(Heap *heap){
    HeapValues values;
    if (heap->lastPos != -1){
         values = heap->values[0];
        heap->values[0] = heap->values[heap->lastPos-1];
        heap->values[heap->lastPos-1].value = 0;
        heap->lastPos = heap->lastPos - 1;
        heapifyDown(heap,0);
    }
    return values;
}