typedef struct H{
    float *values;
    int lastPos;
}Heap;


void insertInHeap(Heap *heap, float value);
Heap *initHeap();
void *heapifyUp(Heap *heap,int posActual);
