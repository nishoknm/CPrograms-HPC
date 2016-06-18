/*
     This Program computes the maximum using parallel pThread  pairwise max algorithm
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Param structure to be passed for each thread
typedef struct params { 
     int *arr;
     int tid;
     int size;
     int nthreads;
     double *time;
} params;

/*
     Function declarations
*/
int pairmax(int *arr, int startIndex, int endIndex);
void *threadParallelSum( void *ptr );
double maximum(double a, double b);
long upper_power_of_two(long v);

static int globalmax = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

/*
     Allows the user to select integer files from three different selections and also allows user to select the number of Threads.
     Note: size of the files (number of integers) must be divisible by the number of threads
     TODO: implement a more generic integer sizes and thread numbers
*/
int main()
{
     int fileno, nthreads, size;
     char *file;
     printf("Select the file name: (Press 1 or 2 or 3)\n 1.1 million entries\n 2.2 million entries\n 3.4 million entries\n");                                                                                                                                                                
     scanf("%d", &fileno);
     switch(fileno){
          case 1:
               printf("File one\n");
               size = 100000;
               file = "random100000.txt";
               break;
          case 2:
               printf("File two\n");
               size = 200000;
               file = "random200000.txt";
               break;
          case 3:
               printf("File three\n");
               size = 400000;
               file = "random400000.txt";
               break;
          case 4:
               printf("File three\n");
               size = 32;
               file = "random32.txt";
               break;
     }
     printf("Enter the number of threads : (1, 2, 4, 8, 16)\n");
     scanf("%d", &nthreads);
     
     int arr[size];
     double time[nthreads], globalTime;
     FILE *myFile;
     myFile = fopen(file, "r");
     int i = 0;
     int j = 0 ;

     fscanf (myFile, "%d", &i);    
     while (!feof (myFile))
     {  
          arr[j] = i;
          j++;
          fscanf (myFile, "%d", &i);    
     }
     fclose (myFile);

     pthread_t threads[nthreads];
     params *thread_params = (params*) malloc(nthreads*sizeof(params));
     for( i = 0; i < nthreads; i++)
     { 
          thread_params[i].arr = arr;
          thread_params[i].tid = i;
          thread_params[i].size = size;
          thread_params[i].nthreads = nthreads;
          thread_params[i].time = time;
          pthread_create(&threads[i], NULL, threadParallelSum, (void*) &thread_params[i]);
     }
     for( i = 0; i < nthreads; i++)
     {
          pthread_join(threads[i], NULL);
     }
     for(i = 0; i < nthreads; i++){
          globalTime = maximum(globalTime,time[i]);
     }
     printf("globalmax : %d , and GlobalTime : %f seconds\n", globalmax, globalTime);
     exit(0);
     return 0;
}

/*
     Thread function
*/
void *threadParallelSum( void *ptr )
{
     clock_t startT, endT;
     double cpu_time_used;
     params *p = (params *) ptr;
     int tid = p->tid;
     int chunk_size = (p -> size / p -> nthreads);
     int start = tid * chunk_size;
     int end = start + chunk_size;
     startT = clock();
     int threadmax = pairmax(p->arr, start, end-1);
     pthread_mutex_lock( &mutex1 );
     globalmax = maximum(threadmax,globalmax);
     pthread_mutex_unlock( &mutex1 );
     endT = clock();
     cpu_time_used = ((double) (endT - startT)) / CLOCKS_PER_SEC;
     p->time[tid] = cpu_time_used;
     printf("Time taken by individual thread %d is %f seconds\n", tid, cpu_time_used);
     return 0;
}

/*
     Pairwise max - computes the pair maximums using recursive algorithm
     *arr - array pointer
     startindex - starting index of the array
     endindex - ending index of the array
     return - max (int)
*/
int pairmax(int *arr, int startIndex, int endIndex)
{
     if(endIndex-startIndex == 1)
     {
          return maximum(arr[startIndex], arr[endIndex]);
     }
     else if (endIndex-startIndex == 0)
     {
          return maximum(arr[endIndex],arr[endIndex]);
     }
     int m = upper_power_of_two((endIndex-startIndex)+1);
     if( m == (endIndex-startIndex)+1)
     {
          m = ((endIndex-startIndex)+1)/2;
     }
     int left = pairmax(arr,startIndex,startIndex+m-1);
     int right = pairmax(arr,startIndex+m,endIndex);
     return maximum(left,right);
}

/*
     bit twiddling nearest power of 2 which is used to build binary tree from leaves to root,
     for instance, array having 10 elements, the spilt would be 8 and 2 for building Btree.
*/
long upper_power_of_two(long v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return (v >> 1)+1;
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