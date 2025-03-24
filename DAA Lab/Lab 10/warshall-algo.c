// C Program for Warshall's Algorithm
#include <stdio.h>

// Number of vertices in the graph
#define V 4

// A function to print the solution matrix
void printSolution(int reach[][V]);

// Solves the transitive closure problem using Warshall's algorithm
void warshallAlgo(int reach[][V])
{
    int i, j, k;

    // Adding vertices one by one to the set of intermediate vertices
    for (k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                // If vertex k is reachable from i and j is reachable from k,
                // then j is reachable from i
                if (reach[i][k] == 1 && reach[k][j] == 1) {
                    reach[i][j] = 1; // Mark as reachable
                }
            }
        }
    }

    printSolution(reach);
}

/* A utility function to print solution */
void printSolution(int reach[][V])
{
    printf(
        "The following matrix shows the transitive closure"
        " of the graph (1 means reachable, 0 means not reachable):\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%7d", reach[i][j]);
        }
        printf("\n");
    }
}

// Driver's code
int main()
{
    int graph[V][V];

    printf("Enter the adjacency matrix for the graph (use 0 for no edge and 1 for an edge):\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            scanf("%d", &graph[i][j]);
            // Ensure the input is either 0 or 1
            if (graph[i][j] != 0 && graph[i][j] != 1) {
                printf("Invalid input! Please enter 0 or 1.\n");
                return 1; // Exit on invalid input
            }
        }
    }

    // Function call
    warshallAlgo(graph);
    return 0;
}