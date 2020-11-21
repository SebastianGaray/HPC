typedef struct HValues{
    float value;
    int originalListIndex;
}HeapValues;


typedef struct H{
    HeapValues *values;
    int lastPos;
    int size;
}Heap;


void insertInHeap(Heap *heap, float value, int originalListIndex);
Heap *initHeap(int size);
void heapifyUp(Heap *heap,int posActual);
HeapValues deleteFromHeap(Heap *heap);