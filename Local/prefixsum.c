#include "stdio.h"
#include "stdlib.h"

struct node
{
	int value, sumval, prefix;
	struct node *left;
	struct node *right;
};

node* newNode(int value, struct node *left, struct node *right);
node* pairsum(int *arr, int startIndex, int endIndex);
node* prefixsum(int parent, struct node *root);
unsigned long upper_power_of_two(unsigned long v);

int main()
{
	int arr[10000];
	FILE *myFile;
    myFile = fopen("random10000.txt", "r");
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

	node *finalsum = pairsum(arr,0,9999);
	node *finalprefix = prefixsum(0,finalsum);
	printf("Final Sum is : %d\n", finalsum->value);
	return 0;
}

node* pairsum(int *arr, int startIndex, int endIndex)
{
	if(endIndex-startIndex == 1)
	{
		node *left = newNode(arr[startIndex],NULL,NULL);
		node *right = newNode(arr[endIndex],NULL,NULL);
		node *sum = newNode(arr[startIndex]+arr[endIndex],left,right);
		return sum;
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
	node *leftsum = pairsum(arr,startIndex,startIndex+m-1);
	node *rightsum = pairsum(arr,startIndex+m,endIndex);
	node *totalsum = newNode(leftsum->value+rightsum->value,leftsum,rightsum);
	return totalsum;
}

node* prefixsum(int parent, struct node *root)
{
	if(root->left == NULL && root->right == NULL)
	{
		root->sumval = parent+root->value;
		printf("Array value : %d and Cumulative sum : %d\n",root->value ,root->sumval);
		return root;
	}
	else
	{
		root->prefix = parent;
		prefixsum(root->prefix,root->left);
		root->sumval = parent+root->left->value;
		prefixsum(root->sumval,root->right);
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

unsigned long upper_power_of_two(unsigned long v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return (v >> 1)+1;
}