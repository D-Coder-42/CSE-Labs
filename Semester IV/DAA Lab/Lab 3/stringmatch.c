#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stringmatch (char* str, char* sub, int* iter) {
	int n = strlen(str), m = strlen(sub);
	int i, j, flag;
	for (i = 0; i < n-m+1; i++) {
		flag = 0;
		for (j = 0; j < m; j++) {
			(*iter)++;
			if (sub[j] != str[i+j]) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) return i+1;
	}
	return -1;
}

int main () {
	char str[100], sub[50];
	int iter = 0;
	printf("Enter string: ");
	scanf(" %[^\n]", str);
	printf("Enter substring to be matched: ");
	scanf(" %[^\n]", sub);

	int pos = stringmatch(str, sub, &iter);
	if (pos == -1) {
		printf("Substring not found in string\n");
	} else {
		printf("Substring found in string at position %d\n", pos);
	}
	printf("Operation count: %d\n", iter);
	return 0;
}