#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for tree nodes
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Function to create a new tree node
struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to insert a node based on the given path
void insertNode(struct Node** root, int value, const char* path) {
    if (*root == NULL) {
        *root = newNode(value);
        return;
    }

    struct Node* current = *root;

    // Traverse the tree according to the path
    for (int i = 0; i < strlen(path); i++) {
        if (path[i] == 'L') {
            if (current->left == NULL) {
                current->left = newNode(0); // Create a placeholder node
            }
            current = current->left;
        } else if (path[i] == 'R') {
            if (current->right == NULL) {
                current->right = newNode(0); // Create a placeholder node
            }
            current = current->right;
        } else {
            printf("Invalid character in path. Use 'L' or 'R'.\n");
            return; // Invalid character
        }
    }

    // Check if the current node already has a value
    if (current->data != 0) { // Assuming 0 is not a valid data value
        printf("Node already exists at the specified path. Try again.\n");
        return; // Do not overwrite existing node
    }

    // Assign the value to the last node in the path
    current->data = value;
}

// Function to find the closest common ancestor
struct Node* findClosestCommonAncestor(struct Node* root, int n1, int n2) {
    // Base case: if root is NULL or matches one of the values
    if (root == NULL || root->data == n1 || root->data == n2) {
        return root;
    }

    // Recur for left and right subtrees
    struct Node* left = findClosestCommonAncestor(root->left, n1, n2);
    struct Node* right = findClosestCommonAncestor(root->right, n1, n2);

    // If both left and right are not NULL, this is the LCA
    if (left && right) {
        return root;
    }

    // Otherwise return the non-NULL child
    return (left != NULL) ? left : right;
}

// Function to display the tree (in-order traversal)
void displayTree(struct Node* root, int space) {
    if (root == NULL) {
        return;
    }

    // Increase distance between levels
    space += 10;

    // Process right child first
    displayTree(root->right, space);

    // Print current node after space
    printf("\n");
    for (int i = 10; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->data);

    // Process left child
    displayTree(root->left, space);
}

// Function to free the allocated memory for the tree
void freeTree(struct Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    struct Node* root = NULL;

    int numNodes;
    printf("Enter number of nodes to insert: ");
    scanf("%d", &numNodes);

    for (int i = 0; i < numNodes; i++) {
        int value;
        char path[100];

        printf("Enter node value: ");
        scanf("%d", &value);
        
        // Handle root node insertion separately
        if (root == NULL) {
            root = newNode(value);
            printf("Inserted root node: %d\n", value);
            displayTree(root, 0);
            continue;
        }

        while (1) {  // Loop until a valid insertion occurs
            printf("Enter path (L for left, R for right): ");
            scanf("%s", path);

            // Validate and insert the node based on the provided path
            insertNode(&root, value, path);

            // Check if the insertion was successful
            struct Node* current = root;
            for (int j = 0; j < strlen(path); j++) {
                if (path[j] == 'L') {
                    current = current->left;
                } else if (path[j] == 'R') {
                    current = current->right;
                }
            }

            // If the current node has the new value, insertion was successful
            if (current != NULL && current->data == value) {
                break; // Exit loop on successful insertion
            } else {
                printf("Invalid path or insertion failed. Try again.\n");
            }
        }

        // Display the tree after each insertion
        printf("Tree after insertion:\n");
        displayTree(root, 0);
    }

    int n1, n2;

    printf("Enter two nodes to find their Closest Common Ancestor: ");
    scanf("%d %d", &n1, &n2);

    struct Node* cca = findClosestCommonAncestor(root, n1, n2);

    if (cca != NULL) {
        printf("Closest Common Ancestor of %d and %d is %d\n", n1, n2, cca->data);
    } else {
        printf("No common ancestor found.\n");
    }

    // Free allocated memory
    freeTree(root);

    return 0;
}