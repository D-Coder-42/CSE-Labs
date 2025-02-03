#include <stdio.h>
#include <stdlib.h>
#include "GraphFuncs.h"

// Function prototypes
void topologicalSortSourceRemoval(Graph g);

// Main function
int main() {
    int n;
    printf("Enter number of vertices: ");
    scanf("%d", &n);

    Graph g = createGraph(n);
    topologicalSortSourceRemoval(g);

    // Free allocated memory
    free(g.nodes);
    for (int i = 0; i < n; i++) {
        Node *temp = g.adjLists[i];
        while (temp) {
            Node *toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(g.adjLists);

    return 0;
}

// Function to perform topological sort using source removal technique
void topologicalSortSourceRemoval(Graph g) {
    int *indegree = (int *)calloc(g.n, sizeof(int));
    int *visited = (int *)calloc(g.n, sizeof(int));
    int count = 0;

    // Calculate in-degrees of all vertices
    for (int i = 0; i < g.n; i++) {
        Node *adjList = g.adjLists[i];
        while (adjList != NULL) {
            indegree[adjList->gNode.index]++;
            adjList = adjList->next;
        }
    }

    printf("Topological Sort: ");
    
    // Process vertices with in-degree of 0
    for (int i = 0; count < g.n; ) {
        if (indegree[i] == 0 && !visited[i]) {
            printf("%d ", g.nodes[i].val);
            visited[i] = 1; // Mark as visited
            count++;

            // Remove the vertex and its outgoing edges
            Node *adjList = g.adjLists[i];
            while (adjList != NULL) {
                indegree[adjList->gNode.index]--; // Decrease in-degree of adjacent vertices
                adjList = adjList->next;
            }

            // Reset i to start checking from the beginning again
            i = -1; // To start from the first vertex after processing one
        }
        i++;
        
        // If we reach the end of the array without finding any in-degree of 0, break
        if (i >= g.n) {
            break;
        }
    }

    printf("\n");

    free(indegree);
    free(visited);
}
