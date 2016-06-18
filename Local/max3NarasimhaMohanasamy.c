/*
	This program computes the maximum number using pairwise maximum, prefix maximum algorithm and time taken to find maximum.
	This program first computes the pairwise maximum by building a upward sweep binary tree from leaves to root, 
	then it computes the prefix maximums for each array element by downward sweep.

	Input array is read from the random32.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
	Binary tree node structure
	value - current value
	maxval - computed prefix max
	prefix - prefix of that node (NULL for leaves)
	*left - pointer to left node (NULL for leaves)
	*right - pointer to right node (NULL for leaves)
*/
struct node
{
	int value, maxval, prefix;
	struct node *left;
	struct node *right;
};

/*
	Function declarations
*/
node* newNode(int value, struct node *left, struct node *right);
node* pairmax(int *arr, int startIndex, int endIndex);
node* prefixmax(int parent, struct node *root);
int maximum(int a, int b);
long upper_power_of_two(long v);

int main()
{
	int arr[10000];
	FILE *myFile;
    myFile = fopen("random10000.txt", "r");
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
	node *finalmax = pairmax(arr,0,9999);
	node *finalprefix = prefixmax(0,finalmax);
	end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Final Max is : %d\n", finalmax->value);
	printf("Time took to compute Max is : %f seconds\n", cpu_time_used);
	return 0;
}

/*
	Pairwise max - computes the pair maximums using recursive algorithm and by building binary tree
	*arr - array pointer
	startindex - starting index of the array
	endindex - ending index of the array
	return - root node
*/
node* pairmax(int *arr, int startIndex, int endIndex)
{
	if(endIndex-startIndex == 1)
	{
		node *left = newNode(arr[startIndex],NULL,NULL);
		node *right = newNode(arr[endIndex],NULL,NULL);
		node *max = newNode(maximum(arr[startIndex], arr[endIndex]),left,right);
		return max;
	}
	else if (endIndex-startIndex == 0)
	{
		node *right = newNode(arr[endIndex],NULL,NULL);
		return right;
	}
	int m = upper_power_of_two((endIndex-startIndex)+1);
	if( m == (endIndex-startIndex)+1)
	{
		m = ((endIndex-startIndex)+1)/2;
	}
	node *leftmax = pairmax(arr,startIndex,startIndex+m-1);
	node *rightmax = pairmax(arr,startIndex+m,endIndex);
	node *totalmax = newNode(maximum(leftmax->value, rightmax->value),leftmax,rightmax);
	return totalmax;
}

/*
	Prefix maximum - computes prefix maximums for all the leaf nodes (input array) by downward sweep
	parent - initial value passed to root as parent, for instance 0 if positive numbers where used
	*root - root node of the binary tree
*/
node* prefixmax(int parent, struct node *root)
{
	if(root->left == NULL && root->right == NULL)
	{
		root->maxval = maximum(parent, root->value);
		printf("Array value : %d and Cumulative max : %d\n",root->value ,root->maxval);
		return root;
	}
	else
	{
		root->prefix = parent;
		prefixmax(root->prefix,root->left);
		root->maxval = maximum(parent, root->left->value);
		prefixmax(root->maxval,root->right);
	}
}

/*
	Function for creating new node - used in pairwise max while building binary tree
*/
node* newNode(int value, struct node *left, struct node *right)
{
	node *curr = (struct node*) malloc(sizeof(struct node));
	if(left == NULL){
		curr->left = curr->right = NULL;
	}
	else
	{
		curr->left = left;
	}
	if(right == NULL){
		curr->right = NULL;
	}
	else
	{
		curr->right = right;
	}
	curr->value = value;
	return curr;
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