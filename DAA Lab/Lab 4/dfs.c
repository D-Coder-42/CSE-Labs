#include <stdio.h>
#include <stdlib.h>
#include "GraphFuncs.h"

void push(Stack *s, GraphNode n) {
    s->nodes[++(s->tos)] = n;
}

GraphNode pop(Stack *s) {
    return s->nodes[(s->tos)--];
}

void dfs(Graph g, int x, int visited[], int *pushOrder, int *popOrder, int *pushIndex, int *popIndex) {
    Stack s = {.tos = -1};
    GraphNode gtemp;

    push(&s, g.nodes[0]);
    visited[0] = 1;
    pushOrder[(*pushIndex)++] = g.nodes[0].index;

    while (s.tos != -1) {
        gtemp = pop(&s);
        popOrder[(*popIndex)++] = gtemp.index;

        if (gtemp.val == x) {
            printf("Value found at node %d\n", gtemp.index + 1);
            return;
        }

        Node *temp = g.adjLists[gtemp.index];
        while (temp) {
            if (!visited[temp->gNode.index]) {
                push(&s, temp->gNode);
                visited[temp->gNode.index] = 1;
                pushOrder[(*pushIndex)++] = temp->gNode.index;
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

    int *pushOrder = malloc(n * sizeof(int));
    int *popOrder = malloc(n * sizeof(int));
    int pushIndex = 0, popIndex = 0;

    dfs(g, x, visited, pushOrder, popOrder, &pushIndex, &popIndex);

    printf("\nPush Order: ");
    for (int i = 0; i < pushIndex; i++) {
        printf("%d ", pushOrder[i] + 1);
    }
    printf("\nPop Order: ");
    for (int i = 0; i < popIndex; i++) {
        printf("%d ", popOrder[i] + 1);
    }
    printf("\n");

    free(visited);
    free(pushOrder);
    free(popOrder);
    return 0;
}