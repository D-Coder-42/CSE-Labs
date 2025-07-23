#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define 'node' using typedef
typedef struct node {
    char* key;
    char* value;
    struct node* next;
} Node;

// Define 'hashMap' using typedef
typedef struct hashMap {
    int numOfElements, capacity;
    Node** arr;
} HashMap;

int key_comparisons = 0;  // Global variable to track the number of key comparisons during search

// Function to set node fields
void setNode(Node* node, char* key, char* value)
{
    node->key = strdup(key);  // Duplicate the key string
    node->value = strdup(value);  // Duplicate the value string
    node->next = NULL;
}

// Function to initialize the hash map
void initializeHashMap(HashMap* mp)
{
    mp->capacity = 100;
    mp->numOfElements = 0;
    mp->arr = (Node**)malloc(sizeof(Node*) * mp->capacity);
    for (int i = 0; i < mp->capacity; i++) {
        mp->arr[i] = NULL;
    }
}

// Hash function to calculate index for key
int hashFunction(HashMap* mp, char* key)
{
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(key); i++) {
        sum = (sum * factor + key[i]) % mp->capacity;
    }
    return sum % mp->capacity;
}

// Insert a key-value pair into the hash map
int insert(HashMap* mp, char* key, char* value)
{
    int bucketIndex = hashFunction(mp, key);
    Node* newNode = (Node*)malloc(sizeof(Node));
    setNode(newNode, key, value);

    Node* currentNode = mp->arr[bucketIndex];

    while (currentNode != NULL) {
        if (strcmp(currentNode->key, key) == 0) {
            free(newNode->key);
            free(newNode->value);
            free(newNode);
            return 0;  // Key already exists, return 0
        }
        currentNode = currentNode->next;
    }

    newNode->next = mp->arr[bucketIndex];
    mp->arr[bucketIndex] = newNode;
    mp->numOfElements++;
    return 1;
}

// Delete a key-value pair from the hash map
int delete(HashMap* mp, char* key)
{
    int bucketIndex = hashFunction(mp, key);
    Node* prevNode = NULL;
    Node* currNode = mp->arr[bucketIndex];

    while (currNode != NULL) {
        if (strcmp(currNode->key, key) == 0) {
            if (prevNode == NULL) {
                mp->arr[bucketIndex] = currNode->next;
            } else {
                prevNode->next = currNode->next;
            }
            free(currNode->key);
            free(currNode->value);
            free(currNode);
            mp->numOfElements--;
            return 1;  // Key found and deleted
        }
        prevNode = currNode;
        currNode = currNode->next;
    }

    return 0;  // Key not found
}

// Search for a key in the hash map
char* search(HashMap* mp, char* key)
{
    int bucketIndex = hashFunction(mp, key);
    Node* bucketHead = mp->arr[bucketIndex];

    // Reset key_comparisons counter before starting the search
    key_comparisons = 0;

    while (bucketHead != NULL) {
        key_comparisons++;  // Increment the counter each time a comparison is made
        if (strcmp(bucketHead->key, key) == 0) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }

    return "Oops! No data found.\n";  // Return error message if key is not found
}

// Display menu options
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Insert key-value pair\n");
    printf("2. Search by key\n");
    printf("3. Delete key-value pair\n");
    printf("0. Exit\n");
}

// Main function to run the menu-driven program
int main()
{
    HashMap* mp = (HashMap*)malloc(sizeof(HashMap));
    initializeHashMap(mp);

    int choice;
    char key[100], value[100];

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline from the input buffer

        switch (choice) {
        case 1:
            printf("Enter key: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';  // Remove newline character
            printf("Enter value: ");
            fgets(value, sizeof(value), stdin);
            value[strcspn(value, "\n")] = '\0';  // Remove newline character
            if (insert(mp, key, value)) {
                printf("Key-value pair inserted!\n");
            } else {
                printf("Key already exists. Insertion failed.\n");
            }
            break;
        case 2:
            printf("Enter key to search: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';  // Remove newline character
            printf("Value: %s\n", search(mp, key));
            printf("Key comparisons made during search: %d\n", key_comparisons);  // Output the number of key comparisons
            break;
        case 3:
            printf("Enter key to delete: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';  // Remove newline character
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
