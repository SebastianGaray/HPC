#include <stdio.h>
#include <stdlib.h>

#include "heap.h"
/*
Funcion que obtiene la posicion del padre de mi dato actual
Entrada: La posicion del dato actual
Salida: La posicion del padre de mi dato actual
*/
int getFather (int actualPosition) {
	if (actualPosition == 0){
		return -2;
	}
	if (actualPosition % 2 == 0 ){
		return (actualPosition/2 -1);
	}
	return (actualPosition/2);
}
/*
Funcion que obtiene la posicion del hijo izquierdo
Entrada: La posicion del dato actual
Salida: La posicion del hijo izquierdo de mi dato actual
*/
int getLeft (int actualPosition) {
	return (2*actualPosition +1);
}
/*
Funcion que obtiene la posicion del hijo derecho
Entrada: La posicion del dato actual
Salida: La posicion del hijo derecho de mi dato actual
*/
int getRight (int actualPosition) {
	return (2*actualPosition +2);
}

/*
Funcion que inicializa el heap
Entrada: La cantidad de datos que tendra el heap
Salida: El heap con sus datos inicializados
*/
Heap *initHeap(int size){
    Heap *heap = malloc(sizeof(Heap));
    heap->values = malloc(sizeof(HeapValues)*size);
    heap->lastPos = 0;
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
    // Caso 1 : 
    //  No tengo hijos
    if  ( (sLeftIndex  >= heap->lastPos) &&  (sRightIndex >= heap->lastPos)){
        return;
    }
    // Caso 2:
    // No tengo hijo izquierdo
    else if (sLeftIndex  >= heap->lastPos){
        min =  heap->values[sRightIndex].value;
        minIndex = sRightIndex;
    }
    // Caso 2:
    // No tengo hijo derecho
    else if (sRightIndex >=  heap->lastPos){
        min =  heap->values[sLeftIndex].value;
        minIndex = sLeftIndex;
    }
    // Caso 3:
    // Calcular el menor entre mis hijos
    else {
        min = (heap->values[sLeftIndex].value < heap->values[sRightIndex].value ? heap->values[sLeftIndex].value: heap->values[sRightIndex].value);
        minIndex =  (heap->values[sLeftIndex].value < heap->values[sRightIndex].value ? sLeftIndex: sRightIndex);
    }
    // Se realiza un swap si es que el valor actual es mayor al hijo menor
    if (heap->values[posActual].value > min){
         swap(heap,posActual,minIndex);
         heapifyDown(heap,minIndex);
    }
}

void heapifyUp(Heap *heap, int posActual){
    int fIndex = getFather(posActual);
    if (fIndex != -2 && heap->values[posActual].value < heap->values[fIndex].value){
        swap(heap,posActual,fIndex);
        heapifyUp(heap, fIndex);
    }
}

void insertInHeap(Heap *heap, float value, int originalListIndex){
    heap->values[heap->lastPos].value = value;
    heap->values[heap->lastPos].originalListIndex = originalListIndex;
    heap->lastPos = heap->lastPos + 1;
    if(heap->lastPos > 1){
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