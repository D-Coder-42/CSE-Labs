#include <stdio.h>
#include <stdlib.h>

int comparisonCount = 0;
int mergeCount = 0;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void merge(int arr[], int temp[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left, l;
    while (i <= mid && j <= right) {
        comparisonCount++;
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (l = left; l <= right; l++) {
        arr[l] = temp[l];
        mergeCount++;
    }
}

void mergeSort(int arr[], int temp[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, temp, left, mid);
        mergeSort(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid, right);
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
    int* temp = (int*)malloc(n * sizeof(int));

    printf("Enter the elements of the array: ");
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    mergeSort(arr, temp, 0, n - 1);

    printf("Sorted array: ");
    displayArray(arr, n);

    printf("Total comparisons: %d\n", comparisonCount);
    printf("Total merge operations (copies): %d\n", mergeCount);

    free(arr);
    free(temp);

    return 0;
}
