#include <stdio.h>
#include <stdlib.h>

int comparisonCount = 0;
int swapCount = 0;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    swapCount++;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++) {
        comparisonCount++;
        if (arr[j] < pivot) {
            // i++;
            swap(&arr[++i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void displayArray(int arr[], int size) {
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);
    int* arr = (int*)malloc(n * sizeof(int));
    printf("Enter the elements of the array: ");
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);
    quickSort(arr, 0, n - 1);
    printf("Sorted array: ");
    displayArray(arr, n);
    printf("Total comparisons: %d\n", comparisonCount);
    printf("Total swaps: %d\n", swapCount);
    free(arr);
    return 0;
}
