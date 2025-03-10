#include <stdio.h>
#include <stdlib.h>

void swap(int *x, int *y) { 
    (*x) ^= (*y) ^= (*x) ^= (*y); 
}

void heapify(int arr[], int N, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < N && arr[l] > arr[largest])
        largest = l;

    if (r < N && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, N, largest);
    }
}

void buildHeap(int arr[], int N)
{
    int startIdx = (N / 2) - 1;

    for (int i = startIdx; i >= 0; i--) {
        heapify(arr, N, i);
    }
}

void displayHeapRecursive(int arr[], int N, int index, int level)
{
    if (index >= N)
        return;
    
    displayHeapRecursive(arr, N, 2 * index + 2, level + 1);
    
    for (int i = 0; i < level; i++)
        printf("\t");
    
    printf("%d\n", arr[index]);
    
    displayHeapRecursive(arr, N, 2 * index + 1, level + 1);
}

void displayHeap(int arr[], int N)
{
    printf("\nHeap as Binary Tree (sideways):\n");
    displayHeapRecursive(arr, N, 0, 0);
}

int main()
{
    int N;
    printf("Enter the number of elements: ");
    scanf("%d", &N);
    
    if (N <= 0) {
        printf("Invalid number of elements\n");
        return 1;
    }
    
    int *arr = (int *)malloc(N * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %d elements:\n", N);
    for (int i = 0; i < N; i++) {
        printf("Element %d: ", i+1);
        scanf("%d", &arr[i]);
    }
    
    buildHeap(arr, N);
    
    printf("\nArray representation of Heap is:\n");
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");
    
    displayHeap(arr, N);
    
    free(arr);
    return 0;
}
