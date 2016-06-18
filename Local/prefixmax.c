#include "stdio.h"
#include "stdlib.h"

struct node
{
	int value, maxval, prefix;
	struct node *left;
	struct node *right;
};

node* newNode(int value, struct node *left, struct node *right);
node* pairmax(int *arr, int startIndex, int endIndex);
node* prefixmax(int parent, struct node *root);
int maximum(int a, int b);
long upper_power_of_two(long v);

int main()
{
	int arr[32];
	FILE *myFile;
    myFile = fopen("random32.txt", "r");
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

	node *finalmax = pairmax(arr,0,31);
	node *finalprefix = prefixmax(0,finalmax);
	printf("Final Max is : %d\n", finalmax->value);
	return 0;
}

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

long upper_power_of_two(long v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return (v >> 1)+1;
}

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