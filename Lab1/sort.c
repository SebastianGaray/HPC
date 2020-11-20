#include "sort.h"

void swap(float *a, float *b)
{
	float temp = *a;
	*a = *b;
	*b = temp;
}
 
void sort(float *arr, int n)
{
	int i, j;
	int swapped;
	for (i = 0; i < n - 1; i++)
	{
		swapped = 0;
		for (j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(&arr[j], &arr[j + 1]);
				swapped = 1;
			}
		}
		if (swapped == 0)
			break;
	}
}
