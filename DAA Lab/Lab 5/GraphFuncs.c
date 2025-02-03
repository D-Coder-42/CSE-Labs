#include "GraphFuncs.h"

void insertEnd(Node **list, GraphNode node) {
    Node *new = malloc(sizeof(Node));
    new->gNode = node;
    new->next = NULL;

    if (*list == NULL) {
        *list = new;
        return;
    }

    Node *temp = *list;
    while (temp->next) 
        temp = temp->next;
    temp->next = new;
}

Graph createGraph(int n) {
    Graph g = {0};
    g.n = n;
    g.nodes = malloc(n * sizeof(GraphNode));
    g.adjLists = calloc(n, sizeof(Node *));

    for (int i = 0; i < n; i++) {
        g.nodes[i].index = i;
        printf("Value for vertex %d: ", i + 1);
        scanf("%d", &g.nodes[i].val);
    }

    int edges, src, dest;
    printf("\nEnter number of edges: ");
    scanf("%d", &edges);

    printf("\nEnter edges (source destination):\n");
    for (int i = 0; i < edges; i++) {
        printf("Edge %d: ", i + 1);
        scanf("%d %d", &src, &dest);
        
        src--; 
        dest--;

        if (src < 0 || src >= n || dest < 0 || dest >= n) {
            printf("Invalid vertex! Skipping edge.\n");
            continue;
        }

        insertEnd(&(g.adjLists[src]), g.nodes[dest]);
    }

    printf("\nGraph Details:\n");
    printf("Vertices: %d\n", n);
    for (int i = 0; i < n; i++) {
        printf("Vertex %d (Value: %d) Connections: ", 
               i + 1, g.nodes[i].val);
        
        Node *temp = g.adjLists[i];
        while (temp) {
            printf("%d ", temp->gNode.index + 1);
            temp = temp->next;
        }
        printf("\n");
    }

    return g;
}