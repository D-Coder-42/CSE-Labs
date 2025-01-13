#include <stdio.h>
#include <stdlib.h>

struct node
{
	int info;
	struct node* llink;
	struct node* rlink;
};
typedef struct node* Node;

Node createNode(){
	Node temp = (Node) malloc (sizeof(struct node));
	if (!temp){
		printf("Node creation failed!\n");
		exit(1);
	} else {
		temp->llink = NULL;
		temp->rlink = NULL;
	}
	return temp;
}

Node findInsertNode(Node root, int data) {
	Node new = createNode();
	new->info = data;
	new->llink = new->rlink = NULL;

	if (!root) return new;
	Node curr = root;
	Node prev = NULL;
	while (curr) {
		if (data == curr->info) {
			printf("Key found!\n");
			free(new);
			return root;
		}
		prev = curr;
		curr = data < curr->info? curr->llink : curr->rlink;
	}
	if (data < prev->info) prev->llink = new;
	else prev->rlink = new;
	return root;
}

void preorderTraversal (Node root) {
	if (!root) return;
	printf("%d\t", root->info);
	preorderTraversal(root->llink);
	preorderTraversal(root->rlink);
}
void inorderTraversal (Node root) {
	if (!root) return;
	inorderTraversal(root->llink);
	printf("%d\t", root->info);
	inorderTraversal(root->rlink);
}
void postorderTraversal (Node root) {
	if (!root) return;
	postorderTraversal(root->llink);
	postorderTraversal(root->rlink);
	printf("%d\t", root->info);
}
void displayTree(Node root, int level){
	if (!root) return;
	level += 1;
	displayTree(root->rlink, level);
	for (int i=0;i<level;i++) printf("\t");
	printf("%d\n", root->info);
	displayTree(root->llink, level);
}
void show(Node root) {
	printf("\n");
	displayTree(root, 0);
	printf("\nPreorder:\t");
	preorderTraversal(root);
	printf("\nInorder:\t");
	inorderTraversal(root);
	printf("\nPostorder:\t");
	postorderTraversal(root);
}

int main() {
	int key;
	Node root = createNode();
	printf("Enter root node key: ");
	scanf("%d", &key);
	root->info = key;
	show(root);

	printf("\n\nEnter Ctrl+C to exit.\n____________________");
	while (1) {
		printf("\n\nEnter key: ");
		scanf("%d", &key);
		root = findInsertNode(root, key);
		show(root);
	}
}