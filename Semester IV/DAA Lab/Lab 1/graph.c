#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

// Structure for adjacency list node
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Structure for graph
typedef struct Graph {
    Node* adjList[MAX_NODES];
    int adjMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
} Graph;

// Function to create a new node
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Function to initialize the graph
void initializeGraph(Graph* graph, int n) {
    graph->numNodes = n;
    for (int i = 0; i < n; i++) {
        graph->adjList[i] = NULL;
        for (int j = 0; j < n; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

// Function to check if an edge already exists
int edgeExists(Graph* graph, int src, int dest) {
    // Node* temp = graph->adjList[src];
    // while (temp) {
    //     if (temp->vertex == dest) {
    //         return 1; // Edge exists
    //     }
    //     temp = temp->next;
    // }
    // return 0; // Edge does not exist
    return graph->adjMatrix[src][dest];
}

// Function to add an edge
void addEdge(Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->numNodes || dest < 0 || dest >= graph->numNodes) {
        printf("Invalid edge (%d, %d). Nodes must be between 0 and %d.\n", src, dest, graph->numNodes - 1);
        return;
    }

    if (edgeExists(graph, src, dest)) {
        printf("Edge (%d, %d) already exists. Skipping.\n", src, dest);
        return;
    }

    // Add edge to adjacency list
    Node* newNode = createNode(dest);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // Update adjacency matrix
    graph->adjMatrix[src][dest] = 1;
}

// Function to display the adjacency list
void displayAdjList(Graph* graph) {
    printf("Adjacency List:\n");
    for (int i = 0; i < graph->numNodes; i++) {
        printf("%d: ", i);
        Node* temp = graph->adjList[i];
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

// Function to display the adjacency matrix
void displayAdjMatrix(Graph* graph) {
    printf("\nAdjacency Matrix:\n");
    for (int i = 0; i < graph->numNodes; i++) {
        for (int j = 0; j < graph->numNodes; j++) {
            printf("%d ", graph->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

// Function to free memory allocated for the adjacency list
void freeAdjList(Graph* graph) {
    for (int i = 0; i < graph->numNodes; i++) {
        Node* temp;
        while (graph->adjList[i]) {
            temp = graph->adjList[i];
            graph->adjList[i] = graph->adjList[i]->next;
            free(temp);
        }
    }
}

int main() {
    int n, edges, src, dest;
    Graph graph;

    printf("Enter the number of nodes: ");
    scanf("%d", &n);

    initializeGraph(&graph, n);
    
    while(1) {
        printf("Enter the number of edges: ");
        scanf("%d", &edges);
        if (edges < 0 || edges > n*(n-1)) {
            printf("Invalid number of edges. Try again\n");
        } else break;
    }

    printf("Enter the edges (src dest):\n");
    for (int i = 0; i < edges; i++) {
        scanf("%d %d", &src, &dest);
        addEdge(&graph, src, dest);
    }

    // Display both representations
    displayAdjList(&graph);
    displayAdjMatrix(&graph);

    // Free allocated memory
    freeAdjList(&graph);

    return 0;
}