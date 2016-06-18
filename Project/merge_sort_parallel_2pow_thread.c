#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>

//Param structure to be passed for each thread
typedef struct params { 
     int *arr;
     int tid;
     int startIndex;
     int endIndex;
     int size;
     int *temp;
     int nthreads;
} params;

void merge(int arr[], int temp[], int startIndex, int midIndex, int endIndex);
void recursiveMergesort(int arr[], int temp[], int startIndex, int endIndex);
void parallelMergesort(int arr[], int size, int nthreads);
int isSorted(int arr[], int size);
void printArray(int arr[], int size);
void *threadRecursiveMergesort( void *ptr );

int main()
{
	int fileno, nthreads, size;
    char *file;
    printf("Select the file name: (Press 1 or 2 or 3)\n 1.400000 entries\n 2.1 million entries\n 3.2 million entries\n");                                                                                                                                                                
    scanf("%d", &fileno);
	switch(fileno){
	  case 1:
	       printf("File one\n");
	       size = 400000;
	       file = "random400000.txt";
	       break;
	  case 2:
	       printf("File two\n");
	       size = 1000000;
	       file = "random1000000.txt";
	       break;
	  case 3:
	       printf("File three\n");
	       size = 2000000;
	       file = "random2000000.txt";
	       break;
	  case 4:
	       printf("File three\n");
	       size = 32;
	       file = "random32.txt";
	       break;
	}
	printf("Enter the number of threads : (1, 2, 4, 8, 16)\n");
    scanf("%d", &nthreads);

	struct timeval start, end;
	double diff;

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

	gettimeofday(&start, NULL);
	parallelMergesort(arr, size, nthreads);
	gettimeofday(&end, NULL);
	diff = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000000.0;

	if (!isSorted(arr, size))
	{
		printf("Merge sort unsuccessful\n");
	}
	else
		printf("Successfully Sorted..!\n");
	//printArray(arr, size);
	printf("Time taken to merge sort sequentially is %f seconds\n", diff);

	return 0;
}

void parallelMergesort(int arr[], int size, int nthreads)
{
	int *temp = (int *) malloc(size*sizeof(int));

	pthread_t threadMain;
	struct params thread_params;
	thread_params.arr = arr;
	thread_params.temp = temp;
	thread_params.startIndex = 0;
	thread_params.endIndex = size-1;
	thread_params.size = size;
	thread_params.temp = temp;
	thread_params.nthreads = nthreads;
	pthread_create(&threadMain, NULL, threadRecursiveMergesort, (void*) &thread_params);
	pthread_join(threadMain, NULL);

	free(temp);
}

/*
     Thread function
*/
void *threadRecursiveMergesort( void *ptr )
{
	params *p = (params *) ptr;
	if (p->nthreads <= 0 || p->startIndex == p->endIndex)
	{
		recursiveMergesort(p->arr, p->temp, p->startIndex, p->endIndex);
		pthread_exit(NULL);
	}
	struct params paramsArr[2];
	int mid = (p->startIndex + p->endIndex)/2;

	for (int i = 0; i < 2; i++)
	{
		paramsArr[i].arr = p->arr;
		paramsArr[i].temp = p->temp;
		paramsArr[i].nthreads = p->nthreads - 1;
	}
	paramsArr[0].startIndex = p->startIndex;
	paramsArr[0].endIndex = mid;
	paramsArr[1].startIndex = mid+1;
	paramsArr[1].endIndex = p->endIndex;

	pthread_t threads[2];
	pthread_create(&threads[0], NULL, threadRecursiveMergesort,	(void *) &paramsArr[0]);
	pthread_create(&threads[1], NULL, threadRecursiveMergesort,	(void *) &paramsArr[1]);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	merge(p->arr, p->temp, p->startIndex, mid, p->endIndex);
	pthread_exit(NULL);
	return 0;
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