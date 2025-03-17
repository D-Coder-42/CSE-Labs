#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 20

// Structure to hold key-value pair where both are strings
struct HashNode {
    char *key;
    char *value;
};

int size = 0; // Current size of the hash table
int key_comparisons = 0;  // Number of key comparisons during searches

struct HashNode** arr;  // The hash table (array of pointers to HashNode)
struct HashNode* dummy;  // A dummy node used for deleted slots

// Simple hash function for strings
int hash(char *key) {
    int h = 0;
    while (*key) {
        h = (h * 31 + *key++) % CAPACITY;
    }
    return h;
}

// Insert function: Inserts key-value pair into the hash table
void insert(char *key, char *value) {
    struct HashNode* temp = (struct HashNode*)malloc(sizeof(struct HashNode));
    temp->key = strdup(key);  // Copy the key
    temp->value = strdup(value);  // Copy the value

    int hashIndex = hash(key);

    // Open addressing using linear probing to resolve collisions
    while (arr[hashIndex] != NULL) {
        if (strcmp(arr[hashIndex]->key, key) == 0) {
            int ch;
            // Key already exists, ask the user if they want to update the value
            printf("Key '%s' already exists. Insertion failed. Do you want to update the value?\nEnter choice (1 for Yes, 0 for No): ", key);
            scanf("%d", &ch);
            if (ch == 1) {
                free(arr[hashIndex]->value);  // Free the old value
                arr[hashIndex]->value = strdup(value);  // Update the value
                printf("Key-Value pair updated\n");
            } else {
                printf("Key-Value pair not updated\n");
            }
            free(temp->key);
            free(temp->value);
            free(temp);
            return;
        }
        hashIndex++;
        hashIndex %= CAPACITY;  // Wrap around if index exceeds capacity
    }

    // If the spot is available (either NULL or dummy), insert the new node
    arr[hashIndex] = temp;
    size++;
    printf("Key '%s' inserted successfully.\n", key);
}

// Delete function: Deletes the key-value pair from the hash table
int delete(char *key) {
    int hashIndex = hash(key);

    while (arr[hashIndex] != NULL) {
        if (strcmp(arr[hashIndex]->key, key) == 0) {
            free(arr[hashIndex]->key);  // Free the key
            free(arr[hashIndex]->value);  // Free the value
            free(arr[hashIndex]);  // Free the node
            arr[hashIndex] = dummy;  // Mark the position as deleted
            size--;
            return 1;
        }
        hashIndex++;
        hashIndex %= CAPACITY;  // Wrap around if index exceeds capacity
    }

    return 0;
}

// Find function: Searches for the key in the hash table
char* find(char *key) {
    int hashIndex = hash(key);
    key_comparisons = 0;  // Reset key comparisons every time we search

    while (arr[hashIndex] != NULL) {
        key_comparisons++;  // Increment key comparisons only during the search
        if (strcmp(arr[hashIndex]->key, key) == 0)
            return arr[hashIndex]->value;

        hashIndex++;
        hashIndex %= CAPACITY;  // Wrap around if index exceeds capacity
    }

    return NULL;
}

// Display Menu function: Displays the menu options for the user
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Insert key-value pair\n");
    printf("2. Delete key-value pair\n");
    printf("3. Search by key\n");
    printf("0. Exit\n");
}

int main() {
    arr = (struct HashNode**)malloc(sizeof(struct HashNode*) * CAPACITY);
    for (int i = 0; i < CAPACITY; i++)
        arr[i] = NULL;

    dummy = (struct HashNode*)malloc(sizeof(struct HashNode));
    dummy->key = NULL;
    dummy->value = NULL;

    int choice;
    char key[100], value[100];

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the buffer for string inputs

        switch (choice) {
            case 1:
                printf("Enter key: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;  // Remove the trailing newline
                printf("Enter value: ");
                fgets(value, sizeof(value), stdin);
                value[strcspn(value, "\n")] = 0;  // Remove the trailing newline
                insert(key, value);
                break;

            case 2:
                printf("Enter key to delete: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                if (delete(key))
                    printf("Node with key '%s' deleted successfully.\n", key);
                else
                    printf("Key '%s' does not exist.\n", key);
                break;

            case 3:
                printf("Enter key to find: ");
                fgets(key, sizeof(key), stdin);
                key[strcspn(key, "\n")] = 0;
                char *result = find(key);
                if (result != NULL)
                    printf("Value of key '%s' is '%s'\n", key, result);
                else
                    printf("Key '%s' does not exist.\n", key);
                printf("Number of key comparisons during search: %d\n", key_comparisons);  // Output the key comparisons for the search
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

///////////////////////////////
///// INT KEY-PAIR VALUES /////
///////////////////////////////

/*
#include <stdio.h>
#include <stdlib.h>

struct HashNode {
    int key;
    int value;
};

const int capacity = 20;
int size = 0;
int key_comparisons = 0;  // Variable to keep track of the key comparisons for searches only

struct HashNode** arr;
struct HashNode* dummy;

// Insert function: Inserts key-value pair into the hash table using open addressing with linear probing
void insert(int key, int V)
{
    struct HashNode* temp = (struct HashNode*)malloc(sizeof(struct HashNode));
    temp->key = key;
    temp->value = V;

    int hashIndex = key % capacity;

    // Open addressing using linear probing to resolve collisions
    while (arr[hashIndex] != NULL) {
        if (arr[hashIndex]->key == key) {
            int ch;
            // Key already exists, do not allow insertion
            printf("Key %d already exists. Insertion failed. Do you want to update key?\nEnter choice (1 for Yes, 0 for No): ", key);
            scanf("%d", &ch);
            if (ch == 1) {
                arr[hashIndex]->value = V;
                printf("Key-Value pair updated\n");
            } else
                printf("Key-Value pair not updated\n");
            free(temp);  // Free the temporary node
            return;
        }
        hashIndex++;
        hashIndex %= capacity;  // Wrap around if index exceeds capacity
    }

    // If the spot is available (either NULL or dummy), insert the new node
    arr[hashIndex] = temp;
    size++;
    printf("Key %d inserted successfully.\n", key);
}

// Delete function: Deletes the key-value pair from the hash table
int delete(int key)
{
    int hashIndex = key % capacity;

    while (arr[hashIndex] != NULL) {
        if (arr[hashIndex]->key == key) {
            arr[hashIndex] = dummy;  // Mark the position as deleted
            size--;
            return 1;
        }
        hashIndex++;
        hashIndex %= capacity;  // Wrap around if index exceeds capacity
    }

    return 0;
}

// Find function: Searches for the key in the hash table
int find(int key)
{
    int hashIndex = key % capacity;
    key_comparisons = 0;  // Reset key comparisons every time we search

    while (arr[hashIndex] != NULL) {
        key_comparisons++;  // Increment key comparisons only during the search
        if (arr[hashIndex]->key == key)
            return arr[hashIndex]->value;

        hashIndex++;
        hashIndex %= capacity;  // Wrap around if index exceeds capacity
    }

    return -1;
}

// Display Menu function: Displays the menu options for the user
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Insert key-value pair\n");
    printf("2. Delete key-value pair\n");
    printf("3. Search by key\n");
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
                printf("Number of key comparisons during search: %d\n", key_comparisons);  // Output the key comparisons for the search
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
*/