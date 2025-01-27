#include <stdio.h>
#include <stdlib.h>
#include "GraphFuncs.h"

void enQueue(Queue *q, GraphNode n) {
    if (q->front == -1)
        q->front = 0;
    q->nodes[++(q->rear)] = n;
}

GraphNode deQueue(Queue *q) {
    GraphNode tempNode = q->nodes[(q->front)++];
    
    if (q->front > q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    return tempNode;
}

void bfs(Graph g, int x, int visited[], int *enqueueOrder, int *dequeueOrder, int *enqueueIndex, int *dequeueIndex) {
    Queue q;
    q.nodes = malloc(g.n * sizeof(GraphNode));
    q.front = -1;
    q.rear = -1;

    GraphNode gtemp;

    enQueue(&q, g.nodes[0]);
    visited[0] = 1;
    enqueueOrder[(*enqueueIndex)++] = g.nodes[0].index;

    while (q.front != -1) {
        gtemp = deQueue(&q);
        dequeueOrder[(*dequeueIndex)++] = gtemp.index;

        if (gtemp.val == x) {
            printf("Value found at node %d\n", gtemp.index + 1);
            return;
        }

        Node *temp = g.adjLists[gtemp.index];
        while (temp != NULL) {
            if (visited[temp->gNode.index] == 0) {
                enQueue(&q, temp->gNode);
                visited[temp->gNode.index] = 1;
                enqueueOrder[(*enqueueIndex)++] = temp->gNode.index;
            }
            temp = temp->next;
        }
    }
    printf("Value not found.\n");
}

int main() {
    int n, x;
    printf("Enter number of nodes in graph: ");
    scanf("%d", &n);

    Graph g = createGraph(n);
    int *visited = calloc(n, sizeof(int));

    printf("Enter value to search for: ");
    scanf("%d", &x);

    int *enqueueOrder = malloc(n * sizeof(int));
    int *dequeueOrder = malloc(n * sizeof(int));
    int enqueueIndex = 0, dequeueIndex = 0;

    bfs(g, x, visited, enqueueOrder, dequeueOrder, &enqueueIndex, &dequeueIndex);

    printf("BFS Traversal Order: ");
    for (int i = 0; i < dequeueIndex; i++) {
        printf("%d ", dequeueOrder[i] + 1);
    }
    printf("\n");

    free(visited);
    free(enqueueOrder);
    free(dequeueOrder);
    return 0;
}