#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void merge(int arr[], int temp[], int startIndex, int midIndex, int endIndex);
void recursiveMergesort(int arr[], int temp[], int startIndex, int endIndex);
void mergesort(int arr[], int size);
int isSorted(int arr[], int size);
void printArray(int arr[], int size);

int main()
{
	int fileno, nthreads, size;
    char *file;
    printf("Select the file name: (Press 1 or 2 or 3)\n 1.1 million entries\n 2.2 million entries\n 3.4 million entries\n");                                                                                                                                                                
    scanf("%d", &fileno);
	switch(fileno){
	  case 1:
	       printf("File one\n");
	       size = 1000000;
	       file = "random1000000.txt";
	       break;
	  case 2:
	       printf("File two\n");
	       size = 2000000;
	       file = "random2000000.txt";
	       break;
	  case 3:
	       printf("File three\n");
	       size = 4000000;
	       file = "random4000000.txt";
	       break;
	  case 4:
	       printf("File three\n");
	       size = 32;
	       file = "random32.txt";
	       break;
	}
	clock_t startT, endT;
	double cpu_time_used;
	FILE *myFile;
	myFile = fopen(file, "r");
	int i = 0, j = 0, arr[size];

	fscanf (myFile, "%d", &i);    
	while (!feof (myFile))
	{  
	  arr[j] = i;
	  j++;
	  fscanf (myFile, "%d", &i);    
	}
	fclose (myFile);

	startT = clock();
	mergesort(arr, size);
	endT = clock();
    cpu_time_used = ((double) (endT - startT)) / CLOCKS_PER_SEC;

	if (!isSorted(arr, size))
	{
		printf("Merge sort unsuccessful\n");
	}
	else
		printf("Successfully Sorted..!\n");
	//printArray(arr, size);
	printf("Time taken to merge sort sequentially is %f seconds\n", cpu_time_used);

	return 0;
}

void mergesort(int arr[], int size)
{
	int *temp = (int *) malloc(size*sizeof(int));
	recursiveMergesort(arr, temp, 0, size-1);
	free(temp);
}

void recursiveMergesort(int arr[], int temp[], int startIndex, int endIndex)
{
	if (startIndex == endIndex) return;
	int midIndex = (startIndex + endIndex)/2;
	recursiveMergesort(arr, temp, startIndex, midIndex);
	recursiveMergesort(arr, temp, midIndex+1, endIndex);
	merge(arr, temp, startIndex, midIndex, endIndex);
}

void merge(int arr[], int temp[], int startIndex, int midIndex, int endIndex)
{
	int ai = startIndex, bi = midIndex + 1, i = startIndex;
	while (ai <= midIndex && bi <= endIndex)
	{
		if (arr[ai] <= arr[bi])
		{
			temp[i] = arr[ai];
			ai ++;
		}else
		{
			temp[i] = arr[bi];
			bi ++;
		}
		i ++;
	}
	if (ai > midIndex)
	{
		memcpy(&temp[i], &arr[bi], (endIndex-bi+1)*sizeof(int));
	}else
	{
		memcpy(&temp[i], &arr[ai], (midIndex-ai+1)*sizeof(int));
	}
	memcpy(&arr[startIndex], &temp[startIndex], (endIndex-startIndex+1)*sizeof(int));
}

void printArray(int arr[], int size)
{
	printf("Printing array\n");
	for (int i = 0; i < size; i ++)
	{
		printf("%d\n", arr[i]);
	}
}


int isSorted(int arr[], int size)
{
	for (int i = 1; i < size; i ++)
	{
		if (arr[i] < arr[i-1]) 
		{
			printf("%d < %d\n", arr[i], arr[i-1]);
			return 0;
		}
	}
	return 1;
}