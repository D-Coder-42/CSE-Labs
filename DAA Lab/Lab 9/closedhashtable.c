#include <stdio.h>
#include <stdlib.h>

struct HashNode {
    int key;
    int value;
};

const int capacity = 20;
int size = 0;

struct HashNode** arr;
struct HashNode* dummy;

void insert(int key, int V)
{
    struct HashNode* temp = (struct HashNode*)malloc(sizeof(struct HashNode));
    temp->key = key;
    temp->value = V;

    int hashIndex = key % capacity;

    while (arr[hashIndex] != NULL && arr[hashIndex]->key != key && arr[hashIndex]->key != -1) {
        hashIndex++;
        hashIndex %= capacity;
    }

    if (arr[hashIndex] == NULL || arr[hashIndex]->key == -1)
        size++;

    arr[hashIndex] = temp;
}

int delete(int key)
{
    int hashIndex = key % capacity;

    while (arr[hashIndex] != NULL) {
        if (arr[hashIndex]->key == key) {
            arr[hashIndex] = dummy;
            size--;
            return 1;
        }
        hashIndex++;
        hashIndex %= capacity;
    }

    return 0;
}

int find(int key)
{
    int hashIndex = (key % capacity);

    while (arr[hashIndex] != NULL) {
        if (arr[hashIndex]->key == key)
            return arr[hashIndex]->value;

        hashIndex++;
        hashIndex %= capacity;
    }

    return -1;
}

void displayMenu()
{
    printf("\nMenu:\n");
    printf("1. Insert\n");
    printf("2. Delete\n");
    printf("3. Find\n");
    printf("0. Exit\n");
}

int main()
{
    arr = (struct HashNode**)malloc(sizeof(struct HashNode*) * capacity);
    for (int i = 0; i < capacity; i++)
        arr[i] = NULL;

    dummy = (struct HashNode*)malloc(sizeof(struct HashNode));
    dummy->key = -1;
    dummy->value = -1;

    int choice, key, value;

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                printf("Enter value: ");
                scanf("%d", &value);
                insert(key, value);
                printf("Key %d inserted successfully.\n", key);
                break;

            case 2:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                if (delete(key))
                    printf("Node with key %d deleted successfully.\n", key);
                else
                    printf("Key %d does not exist.\n", key);
                break;

            case 3:
                printf("Enter key to find: ");
                scanf("%d", &key);
                value = find(key);
                if (value != -1)
                    printf("Value of key %d is %d\n", key, value);
                else
                    printf("Key %d does not exist.\n", key);
                break;

            case 0:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (1);

    return 0;
}

///////////////////////////////////////////////////////
///// SHORTENED CODE (WITH STRING KEY-VALUE PAIR) /////
///////////////////////////////////////////////////////

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAP 20

typedef struct { char *k,*v; } Node;
Node **t, *d;  // Table and dummy
int s=0;       // Size

int hash(char *k) {
    int h=0; while(*k) h=(h*31 + *k++)%CAP;
    return h;
}

void ins(char *k, char *v) {
    Node *n=malloc(sizeof(*n));
    n->k=strdup(k); n->v=strdup(v);
    int i=hash(k);
    for(;t[i]&&t[i]!=d&&strcmp(t[i]->k,k);i=(i+1)%CAP);
    if(!t[i]||t[i]==d) s++;
    t[i]=n;
}

int del(char *k) {
    for(int i=hash(k);t[i];i=(i+1)%CAP)
        if(t[i]!=d && !strcmp(t[i]->k,k)) {
            free(t[i]->k); free(t[i]->v); free(t[i]);
            t[i]=d; s--; return 1;
        }
    return 0;
}

char* find(char *k) {
    for(int i=hash(k);t[i];i=(i+1)%CAP)
        if(t[i]!=d && !strcmp(t[i]->k,k)) return t[i]->v;
    return "Not found";
}

int main() {
    t=calloc(CAP,sizeof(*t));
    d=malloc(sizeof(*d)); *d=(Node){0};
    
    int c; char k[100],v[100];
    while(1) {
        printf("\n1.Ins\n2.Del\n3.Find\n0.Exit\nChoice: ");
        scanf("%d%*c",&c); if(!c) break;
        
        printf("Key: "); fgets(k,100,stdin);
        k[strcspn(k,"\n")]=0;
        
        if(c==1) {
            printf("Val: "); fgets(v,100,stdin);
            v[strcspn(v,"\n")]=0;
            ins(k,v); printf("Added\n");
        }
        else if(c==2) printf(del(k)?"Deleted\n":"Missing\n");
        else if(c==3) printf("Value: %s\n",find(k));
    }
}
*/
