//
//  wordsort.c
//  P5
//
//  Created by Andrea Donoso on 11/6/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Global variable Q tracks the index of the array when copying each struct into each index of the array of structs
int Q;


typedef struct binST
{
    char word[21];
    int frequency;
    int depth;
    struct binST*left;
    struct binST*right;
}binST;


void printTree(binST*root);
binST * addToTree(binST*root, char reqWord[], int k);
void query(binST*root, char reqWord[]);
int countNodes(binST * root);
void printArray(binST * array, int length);
void freeTree(binST * root);
void copyToArray(binST * root, binST * array);
void merge(binST * array, int low, int high, int middle);
void mSort(binST * array, int low, int high);


int main(void)
{
    
    // Gets the number of words and queries desired
    int n;
    scanf("%d", &n);
    
    int act=0;
    char reqWord[20];
    binST*root=NULL;
    
    // Gets the desired action and word from the user
    for(int i=0; i<n; i++)
    {
        scanf("%d %s", &act, reqWord);
        
/*
                for(int i=0; i<100000; i++)
                {
                    
                    //printf("INSERT %d\n",i+1);
                    act=1;
                    if(i%5==0)
                        strcpy(reqWord, "dog");
                    else if (i%5==1)
                        strcpy(reqWord, "cat");
                    else if (i%5==2)
                        strcpy(reqWord, "horse");
                    else if (i%5==3)
                        strcpy(reqWord, "bird");
                    else if(i%5==4)
                        strcpy(reqWord, "lizard");
                    //printf("%d %s\n", act, reqWord);
                    if(act==1)
                        root=addToTree(root, reqWord,0);
                    if(i>99999)
                        exit(0);
                }
                //printf("\n\n\n\n\n\n\n\n\n-------------------------------------------------\n\n\n\n\n\n\n\n\n\n");
                for(int j=0; j<100000; j++)
                {
                    printf("QUERY %d\n",j+1);
                    act=2;
                    if(j%5==0)
                        strcpy(reqWord, "dog");
                    else if (j%5==1)
                        strcpy(reqWord, "cat");
                    else if (j%5==2)
                        strcpy(reqWord, "horse");
                    else if (j%5==3)
                        strcpy(reqWord, "bird");
                    else if(j%5==4)
                        strcpy(reqWord, "lizard");
                    printf("%d %s\n", act, reqWord);
                    if(act==2)
                        query(root, reqWord);
                    if(j>99999)
                        exit(0);
                }
                printf("\n\n-------------------------------------------------\n\n");
*/
  
        
        // If 1, then add it to the tree
        if(act==1)
            root=addToTree(root, reqWord,0);
        // If 2, then its a query
        else if(act==2)
            query(root, reqWord);
    }
    
    
    // Prints out the binary search tree: word, frequency, and depth of each node
    //printTree(root);
    
    // Dynamically allocates memory for the array of structs. The length of the array should be equal to the number of nodes in the tree
    int numNodesInTree=countNodes(root);
    binST*array=malloc(sizeof(binST)*numNodesInTree);
    
    // Sets the index of the array to 0 and calls function to copy the items from the binary search tree to the the array
    Q=0;
    copyToArray(root, array);
    
    // Prints unsorted array
    //printf("\nUnsorted Array:\n");
    //printArray(array, numNodesInTree);
    
    // Sorts the array by descending frequency, and if their frequency is equal, the alphabetically first word comes first
    mSort(array, 0, numNodesInTree-1);
    
    // Prints the she sorted array
    //printf("\nSorted Array:\n");
    printArray(array,numNodesInTree);
    
    
    // Frees dynamically allocated memory (BST and array of structs)
    freeTree(root);
    free(array);
    
    return 0;
}



// Either inserts words to the tree or increments the frequency of a repeated word and keeps track of depth (k) from the root
binST * addToTree(binST*root, char reqWord[], int k)
{
    // Base case
    if(root==NULL)
    {
        // Dynamically allocates memory for the binary search tree and initializes it
        root=(binST*)malloc(sizeof(binST));
        strcpy(root->word, reqWord);
        root->frequency=1;
        root->depth=k;
        root->left=NULL;
        root->right=NULL;
        return root;
    }
    
    // Update the depth of the node
    root->depth=k;
    
    // If the string compare of the requested word is the same as the root, increment its frequency. Depth stays the same.
    if (strcmp(reqWord, root->word)==0)
        root->frequency++;
    
    // If the requested word is less than the root, make it the left node of root and increment its depth
    else if(strcmp(reqWord, root->word)<0)
    {
        // Since we are moving to another node, increment depth k
        k++;
        root->left=addToTree(root->left, reqWord, k);
    }
    
    // If the requested word is more than the root, make it the right node of root and increment its depth
    else if (strcmp(reqWord, root->word)>0)
    {
        // Since we are moving to another node, increment depth k
        k++;
        root->right=addToTree(root->right, reqWord, k);
    }
    
    // Returns the root of the binary search tree
    return root;
}



void query(binST*root, char reqWord[])
{
    if(root==NULL)
        printf("-1  -1\n");
    else if(strcmp(reqWord, root->word)==0)
        printf("%d  %d\n", root->frequency, root->depth);
    else if(strcmp(reqWord, root->word)<0)
        query(root->left, reqWord);
    else if(strcmp(reqWord, root->word)>0)
        query(root->right, reqWord);
}



// Prints out the info of each node in the binary search tree in a preorder traversal
void printTree(binST*root)
{
    // If the root is not empty, print it
    if(root!=NULL)
    {
        printf("%s  f: %d  d: %d\n", root->word, root->frequency,root->depth);
        printTree(root->left);
        printTree(root->right);
    }
}



void copyToArray(binST * root, binST * array)
{
    if(root==NULL) return;
    
    // Increments the index AFTER array[i] has been assigned
    array[Q++]=*root;
    
    copyToArray(root->left, array);
    copyToArray(root->right, array);
}



int countNodes(binST * root)
{
    // If the root is null, return 0
    if(root==NULL)return 0;
 
    // Counts the number of nodes to the left of root
    int lNodes=countNodes(root->left);
    
    // Counts the numnber of node to the right of root
    int rNodes=countNodes(root->right);
    
    // If the root is not null then there is 1 node. Returns 1 (root node) + left nodes + right nodes
    return 1+lNodes+rNodes;
}


void printArray(binST * array, int length)
{
    for(int i=0; i<length; i++)
    {
        printf("%s %d\n", array[i].word, array[i].frequency);
        
    }
}



void merge(binST * array, int low, int high, int middle)
{
    // Gets the length of each string
    int length=high-low+1;
    
    // Dynamically allocates memory for the auxiliary array and zeroes it out
    binST * temp=calloc(length, sizeof(binST));
    
    // Sets counter1 to the beginning of the first list and counter 2 at the beginning of the second list
    int counter1=low;
    int counter2=middle;
    
    // Sets the index of the auxiliary array to 0
    int index=0;
    
    
    // While there are values left to copy
    while (counter1<middle || counter2<=high)
    {
        // If in bounds and if the frequency of the word at counter 1 is greater than the frequency of the word at counter 2
        if(counter2>high || (counter1<middle && array[counter1].frequency > array[counter2].frequency))
        {
            //printf("%s %d > %d %s\n", array[counter1].word, array[counter1].frequency, array[counter2].frequency, array[counter2].word);
            temp[index]=array[counter1];
            //printf("Temp[%d] %s %d\n", index, array[counter1].word, array[counter1].frequency);
            counter1++;
            index++;
        }
        // If in bounds and if the frequency of the words are the same, then the word that comes alphabetically first goes into the auxilliary array first
        else if(counter2>high || (counter1<middle && (array[counter1].frequency == array[counter2].frequency) && (strcmp(array[counter1].word, array[counter2].word)<0)))
        {
            //printf("%s %d = %d %s \n", array[counter1].word, array[counter1].frequency, array[counter2].frequency, array[counter2].word);
            temp[index]=array[counter1];
            //printf("Temp[%d] %s %d\n", index, array[counter1].word, array[counter1].frequency);
            counter1++;
            index++;
        }
        // If the frequency of the word at counter 1 is less than the frequency of the word at counter 2, or, if both frequencies are equal then the one that does not come alphabetically first gets put into the auxiliary array
        else
        {
            //printf("%s %d <= %d %s\n", array[counter1].word, array[counter1].frequency, array[counter2].frequency, array[counter2].word);
            temp[index]=array[counter2];
            //printf("Temp[%d] %s %d\n", index, array[counter2].word, array[counter2].frequency);
            counter2++;
            index++;
        }
    }
    
    // Copies the sorted items from the auxiliary array back into the original array
    for(int i=low; i<=high; i++)
        array[i]=temp[i-low];
    
    // Frees the memory of the auxiliary array
    free(temp);
}



void mSort(binST * array, int low, int high)
{
    // Splits the array in half
    int mid;
    
    // Merge sort left and right side of the array until there is 1 element remainding. If there is only one element then its already sorted
    if(low<high)
    {
        mid=(high+low)/2;
        // Sorts left of the array
        mSort(array, low, mid);
        
        // Sorts right of the array
        mSort(array, mid+1, high);
        
        // Sorts and merges the arrays
        merge(array, low, high, mid+1);
    }
}



void freeTree(binST * root)
{
    if(root==NULL)return;
    
    freeTree(root->left);
    freeTree(root->right);
    
    free(root);
}
