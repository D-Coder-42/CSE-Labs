#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char* key;
    char* value;
    struct node* next;
};

void setNode(struct node* node, char* key, char* value)
{
    node->key = key;
    node->value = value;
    node->next = NULL;
}

struct hashMap {
    int numOfElements, capacity;
    struct node** arr;
};

void initializeHashMap(struct hashMap* mp)
{
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (struct node**)malloc(sizeof(struct node*) * mp->capacity);
}

int hashFunction(struct hashMap* mp, char* key)
{
    int bucketIndex;
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(key); i++) {
        sum = ((sum % mp->capacity) + (((int)key[i]) * factor) % mp->capacity) % mp->capacity;
        factor = ((factor % __INT16_MAX__) * (31 % __INT16_MAX__)) % __INT16_MAX__;
    }
    bucketIndex = sum;
    return bucketIndex;
}

int insert(struct hashMap* mp, char* key, char* value)
{
    int bucketIndex = hashFunction(mp, key);
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    setNode(newNode, key, value);

    struct node* currentNode = mp->arr[bucketIndex];

    while (currentNode != NULL) {
        if (strcmp(currentNode->key, key) == 0) {
            free(newNode);
            return 0;
        }
        currentNode = currentNode->next;
    }

    newNode->next = mp->arr[bucketIndex];
    mp->arr[bucketIndex] = newNode;
    mp->numOfElements++;
    return 1;
}

int delete(struct hashMap* mp, char* key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node* prevNode = NULL;
    struct node* currNode = mp->arr[bucketIndex];

    while (currNode != NULL) {
        if (strcmp(currNode->key, key) == 0) {
            if (prevNode == NULL) {
                mp->arr[bucketIndex] = currNode->next;
            } else {
                prevNode->next = currNode->next;
            }
            free(currNode);
            mp->numOfElements--;
            return 1;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }

    return 0;
}

char* search(struct hashMap* mp, char* key)
{
    int bucketIndex = hashFunction(mp, key);
    struct node* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {
        if (strcmp(bucketHead->key, key) == 0) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }
    char* errorMssg = (char*)malloc(sizeof(char) * 25);
    strcpy(errorMssg, "Oops! No data found.\n");
    return errorMssg;
}

void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Insert key-value pair\n");
    printf("2. Search by key\n");
    printf("3. Delete key-value pair\n");
    printf("0. Exit\n");
}

int main()
{
    struct hashMap* mp = (struct hashMap*)malloc(sizeof(struct hashMap));
    initializeHashMap(mp);

    int choice;
    char key[100], value[100];

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("Enter key: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';
            printf("Enter value: ");
            fgets(value, sizeof(value), stdin);
            value[strcspn(value, "\n")] = '\0';
            if (insert(mp, key, value)) {
                printf("Key-value pair inserted!\n");
            } else {
                printf("Key already exists. Insertion failed.\n");
            }
            break;
        case 2:
            printf("Enter key to search: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';
            printf("Value: %s\n", search(mp, key));
            break;
        case 3:
            printf("Enter key to delete: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';
            if (delete(mp, key)) {
                printf("Key-value pair deleted!\n");
            } else {
                printf("Key not found. Deletion failed.\n");
            }
            break;
        case 0:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (1);

    return 0;
}
