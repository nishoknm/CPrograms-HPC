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
     int size;
     int *temp;
     double *time;
     int nthreads;
} params;

void merge(int arr[], int temp[], int startIndex, int midIndex, int endIndex);
void recursiveMergesort(int arr[], int temp[], int startIndex, int endIndex);
void parallelMergesort(int arr[], int size, int nthreads);
void threadRecursiveMerge(int arr[], int temp[], int mergeFactor, int size);
int isSorted(int arr[], int size);
void printArray(int arr[], int size);
void *threadRecursiveMergesort( void *ptr );
double maximum(double a, double b);

int main()
{
	int fileno, nthreads, size;
    char *file;
    printf("Select the file name: (Press 1 or 2 or 3)\n 1.400000 entries\n 2.1 million entries\n 3.2million entries\n");                                                                                                                                                                
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
	double time[nthreads], globalTime;
	struct timeval startT, endT;
	double diff;
	int *temp = (int *) malloc(size*sizeof(int));

	pthread_t threads[nthreads];
	params *thread_params = (params*) malloc(nthreads*sizeof(params));
	for(int i = 0; i < nthreads; i++)
	{ 
	  thread_params[i].arr = arr;
	  thread_params[i].tid = i;
	  thread_params[i].size = size;
	  thread_params[i].temp = temp;
	  thread_params[i].time = time;
	  thread_params[i].nthreads = nthreads;
	  pthread_create(&threads[i], NULL, threadRecursiveMergesort, (void*) &thread_params[i]);
	}
	for(int i = 0; i < nthreads; i++)
	{
	  pthread_join(threads[i], NULL);
	}
	gettimeofday(&startT, NULL);
	threadRecursiveMerge(arr, temp, nthreads, size);
	gettimeofday(&endT, NULL);
	diff = ((endT.tv_sec * 1000000 + endT.tv_usec) - (startT.tv_sec * 1000000 + startT.tv_usec))/1000000.0;
    for(int i = 0; i < nthreads; i++){
          globalTime = maximum(globalTime,time[i]);
    }
    printf("Total Global time : %f\n", (globalTime+diff));
	free(temp);
}

void threadRecursiveMerge(int arr[], int temp[], int mergeFactor, int size)
{
	if(mergeFactor != 1){
		mergeFactor = mergeFactor/2;
		int chunk_size = size/mergeFactor;
		int chunk_mid = chunk_size/2;
		for(int i=0; i<mergeFactor; i++)
		{
			int start = i * chunk_size;
			int mid = start + chunk_mid;
			int end = start + chunk_size;
			merge(arr, temp, start, mid-1, end-1);
		}
		threadRecursiveMerge(arr, temp, mergeFactor, size);
	}
	//end of merge
}

/*
     Thread function
*/
void *threadRecursiveMergesort( void *ptr )
{
	struct timeval startT, endT;
	double diff;
	params *p = (params *) ptr;
	int tid = p->tid;
	int chunk_size = (p -> size / p -> nthreads);
	int start = tid * chunk_size;
	int end = start + chunk_size;
	gettimeofday(&startT, NULL);
	recursiveMergesort(p->arr, p->temp, start, end-1);
	gettimeofday(&endT, NULL);
	diff = ((endT.tv_sec * 1000000 + endT.tv_usec) - (startT.tv_sec * 1000000 + startT.tv_usec))/1000000.0;
    p->time[tid] = diff;
    printf("Time taken by individual thread %d is %f seconds\n", tid, diff);
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

/*
     Local function to compute maximum of two elements
*/
double maximum(double a , double b)
{
     if(a>b)
     {
          return a;
     }
     else 
     {
          return b;
     }
}