#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
	Function declarations
*/
int pairmax(int *arr, int startIndex, int endIndex);
int maximum(int a, int b);
long upper_power_of_two(long v);

int main()
{
	int size = 400000;
	int arr[size];
	FILE *myFile;
    myFile = fopen("random400000.txt", "r");
    int i = 0;
    int j = 0 ;
    clock_t start, end;
    double cpu_time_used;

  	fscanf (myFile, "%d", &i);    
  	while (!feof (myFile))
    {  
      arr[j] = i;
      j++;
      fscanf (myFile, "%d", &i);    
    }
  	fclose (myFile);
	start = clock();
	int finalmax = pairmax(arr,0,size-1);
	end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Final Max is : %d , Time : %f\n", finalmax, cpu_time_used);
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
int maximum(int a , int b)
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