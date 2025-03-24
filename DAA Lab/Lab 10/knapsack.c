#include <stdio.h>

// Utility function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to find the maximum value that can be obtained
int knapsack(int W, int wt[], int val[], int n) {
    int i, w;
    int K[n + 1][W + 1];

    // Build the K table in a bottom-up manner
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                K[i][w] = 0; // Base case: no items or no capacity
            } else if (wt[i - 1] <= w) {
                // Item can be included
                K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            } else {
                // Item cannot be included
                K[i][w] = K[i - 1][w];
            }
        }
    }

    return K[n][W]; // The maximum value that can be obtained
}

// Driver code
int main() {
    int n, W;

    // Input number of items and maximum weight
    printf("Enter the number of items: ");
    scanf("%d", &n);
    printf("Enter the maximum weight of the knapsack: ");
    scanf("%d", &W);

    int val[n], wt[n];

    // Input values and weights of the items
    printf("Enter the values of the items:\n");
    for (int i = 0; i < n; i++) {
        printf("Value of item %d: ", i + 1);
        scanf("%d", &val[i]);
    }

    printf("Enter the weights of the items:\n");
    for (int i = 0; i < n; i++) {
        printf("Weight of item %d: ", i + 1);
        scanf("%d", &wt[i]);
    }

    // Function call to solve the knapsack problem
    int maxValue = knapsack(W, wt, val, n);
    printf("The maximum value that can be obtained is: %d\n", maxValue);

    return 0;
}