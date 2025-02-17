#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
};

int getHeight(struct Node* n) {
    return (n == NULL) ? 0 : n->height;
}

struct Node* createNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int max(int a, int b) { 
    return (a > b) ? a : b;
}

int getBalanceFactor(struct Node* n) {
    return (n == NULL) ? 0 : getHeight(n->left) - getHeight(n->right);
}

struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

struct Node* insert(struct Node* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void inOrder(struct Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

void printTree(struct Node* root, int space) {
    if (root == NULL)
        return;

    space += 1;
    printTree(root->right, space);

    printf("\n");
    for (int i = 1; i < space; i++)
        printf("\t");
    printf("%d\n", root->key);

    printTree(root->left, space);
}

void findPreSuc(struct Node* root, int key, struct Node** pre, struct Node** suc) {
    if (!root) return;
    findPreSuc(root->left, key, pre, suc);

    if (root->key > key) {        
        if (!*suc || (*suc && (*suc)->key > root->key))
            *suc = root;
    }
    else if (root->key < key) *pre = root;
    findPreSuc(root->right, key, pre, suc);
}

int main() {
    struct Node* root = NULL;
    int value, continueInput, searchKey;

    do {
        printf("Enter a value to insert into the AVL Tree: ");
        scanf("%d", &value);

        root = insert(root, value);

        printf("\nTree after insertion of %d:\n", value);
        printTree(root, 0);
        printf("\n");

        printf("Do you want to insert another value? (1 for Yes, 0 for No): ");
        scanf("%d", &continueInput);

    } while (continueInput != 0);

    printf("\nInorder traversal of the AVL tree: ");
    inOrder(root);
    printf("\n");

    printf("\nEnter a key to find its successor and predecessor: ");
    scanf("%d", &searchKey);

    struct Node* predecessor = NULL;
    struct Node* successor = NULL;
    findPreSuc(root, searchKey, &predecessor, &successor);

    if (successor != NULL)
        printf("Successor of %d: %d\n", searchKey, successor->key);
    else printf("No successor found for %d\n", searchKey);

    if (predecessor != NULL)
        printf("Predecessor of %d: %d\n", searchKey, predecessor->key);
    else printf("No predecessor found for %d\n", searchKey);
    return 0;
}