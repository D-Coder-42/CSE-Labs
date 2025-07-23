#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GraphFuncs.h" // Include your predefined header file

// Function prototypes
void topologicalSortUtil(int v, Graph* graph, bool* visited, Stack* stack);
void topologicalSort(Graph* graph);

int main() {
    int n;
    printf("Enter number of vertices: ");
    scanf("%d", &n);

    Graph g = createGraph(n); // Create the graph

    // Perform topological sort
    topologicalSort(&g);

    // Free allocated memory
    free(g.nodes);
    for (int i = 0; i < g.n; i++) {
        Node* temp = g.adjLists[i];
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(g.adjLists);

    return 0;
}

// Function to perform DFS and topological sorting
void topologicalSortUtil(int v, Graph* graph, bool* visited, Stack* stack) {
    visited[v] = true; // Mark the current node as visited

    Node* adjList = graph->adjLists[v];
    
    while (adjList != NULL) {
        if (!visited[adjList->gNode.index]) {
            topologicalSortUtil(adjList->gNode.index, graph, visited, stack);
        }
        adjList = adjList->next;
    }

    // Push current vertex to stack which stores the result
    stack->nodes[stack->tos++] = graph->nodes[v];
}

// Function to perform Topological Sort
void topologicalSort(Graph* graph) {
    Stack stack;
    stack.tos = 0;

    // Initialize visited array
    bool* visited = (bool*)calloc(graph->n, sizeof(bool));

    // Call the recursive helper function to store Topological Sort starting from all vertices one by one
    for (int i = 0; i < graph->n; i++) {
        if (!visited[i]) {
            topologicalSortUtil(i, graph, visited, &stack);
        }
    }

    // Print the contents of the stack in reverse order
    printf("Topological Sort: ");
    for (int i = stack.tos - 1; i >= 0; i--) {
        printf("%d ", stack.nodes[i].val);
    }
    printf("\n");

    free(visited);
}
