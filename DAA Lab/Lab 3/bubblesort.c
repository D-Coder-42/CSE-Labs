#include <stdio.h>
#include <stdlib.h>

void bubblesort(int* arr, int n, int* opcount) {
	int flag, i, j, temp;
	for (i = 0; i < n-1; i++) {
		flag = 0;
		for (j = 0; j < n-1-i; j++) {
			(*opcount)++;
			if (arr[j] > arr[j+1]) {
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
				flag = 1;
			}
		}
		if (flag == 0) break;
	}
}

int main() {
	int arr[100];
	int n,i, opcount = 0;
	printf("Enter number of elements in array: ");
	scanf("%d", &n);
	printf("Enter elements of array: \n");
	for (i = 0; i<n; i++) {
		scanf("%d", arr+i);
	}
	printf("\nUnsorted array:\n");
	for (i = 0; i<n; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\n");

	bubblesort(arr, n, &opcount);
	printf("\nSorted array:\n");
	for (i = 0; i<n; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\nOperation count: %d\n", opcount);
	return 0;
}