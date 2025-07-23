#include <stdio.h>
#include <stdlib.h>

int opCount = 0; // Global variable to count operations

int sum(int arr[], int n) {
    int total = 0;
    for(int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;
}

void printSubset(int arr[], int subset[], int n) {
    for(int i = 0; i < n; i++) {
        if(subset[i]) printf("%d ", arr[i]);
    }
}

int findSubset(int arr[], int n, int sum, int subset[], int index) {
    opCount++; // Count the operation for the function call
    if (index == n) {
        opCount++; // Count the comparison operation
        return sum == 0; // Check if the remaining sum is zero
    }

    subset[index] = 1; // Include the current element
    opCount++; // Count the operation for including the element
    if (findSubset(arr, n, sum - arr[index], subset, index + 1)) {
        return 1; // Found a valid subset
    }

    subset[index] = 0; // Exclude the current element
    opCount++; // Count the operation for excluding the element
    return findSubset(arr, n, sum, subset, index + 1); // Check without including the current element
}

int partition(int arr[], int n) {
    int totalSum = sum(arr, n);
    if (totalSum % 2 != 0) return 0; // Cannot partition if total sum is odd
    int target = totalSum / 2;
    int subset[n];
    for (int i = 0; i < n; i++) {
        subset[i] = 0; // Initialize subset array
    }
    if (findSubset(arr, n, target, subset, 0)) {
        printf("Subset 1: ");
        printSubset(arr, subset, n);
        printf("\nSubset 2: ");
        for (int i = 0; i < n; i++) {
            if (subset[i] == 0) {
                printf("%d ", arr[i]);
            }
        }
        printf("\n");
        return 1; // Partition found
    }
    return 0; // No partition found
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int arr[50];
    printf("Enter the elements:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    if (partition(arr, n)) {
        printf("Can be partitioned.\n");
    } else {
        printf("Cannot be partitioned.\n");
    }

    // Print the total operation count
    printf("Total operations: %d\n", opCount);
    return 0;
}