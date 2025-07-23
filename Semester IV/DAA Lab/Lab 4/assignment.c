#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void permute(int arr[], int n, int **mat, int l, int r, int *cost, int res[], int *opCount) {
    if (l == r) {
        int tempCost = 0;
        for (int i = 0; i < n; i++) 
            tempCost += mat[i][arr[i]];
        
        (*opCount)++;
        
        if (*cost > tempCost) {
            *cost = tempCost;
            for (int i = 0; i < n; i++) 
                res[i] = arr[i];
        }
    } else {
        for (int i = l; i <= r; i++) {
            swap(&arr[l], &arr[i]);
            permute(arr, n, mat, l + 1, r, cost, res, opCount);
            swap(&arr[l], &arr[i]);
        }
    }
}

int main() {
    int n, i, j, opCount = 0;
    
    printf("Enter the number of employees and jobs: ");
    scanf("%d", &n);
    
    int **mat = malloc(n * sizeof(int *));
    int *arrForPerm = malloc(n * sizeof(int));
    int *res = malloc(n * sizeof(int));
    
    for (i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(int));
        arrForPerm[i] = i;
    }
    
    printf("Enter efficiency matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("Efficiency of employee %d for job %d: ", i + 1, j + 1);
            scanf("%d", &mat[i][j]);
        }
    }
    
    printf("\nEfficiency Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
    
    int cost = INT_MAX;
    
    permute(arrForPerm, n, mat, 0, n - 1, &cost, res, &opCount);
    
    printf("\nMinimum Cost: %d\n", cost);
    printf("Optimal Assignment:\n");
    for (i = 0; i < n; i++)
        printf("Employee %d is assigned to Job %d\n", i + 1, res[i] + 1);
    
    printf("Total operations: %d\n", opCount);
    
    for (i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
    free(arrForPerm);
    free(res);
    
    return 0;
}