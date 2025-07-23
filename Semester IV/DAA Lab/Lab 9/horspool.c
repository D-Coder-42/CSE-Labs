#include <stdio.h>
#include <string.h>

#define MAX_ASCII	256
#define MAX_LENGTH	512

int horspool(char string[], char pattern[], int shiftTable[]);
void genShiftTable(char *,int []);

int main(void)
{
	char text[MAX_LENGTH];
	char pattern[MAX_LENGTH];
    int shiftTable[MAX_ASCII];
	int found;
	printf("Enter the source string: ");
	scanf("%[^\n]", text);
	getchar();
	printf("Enter the pattern string: ");
	scanf("%[^\n]", pattern);
	
	genShiftTable(pattern,shiftTable);
	found = horspool(text,pattern,shiftTable);

	if(found==-1)
		puts("\nMatching substring not found.\n");
	else
		printf("\nMatching substring found at position: %d\n",found+1);

	return 0;
}

void genShiftTable(char p[], int t[]) {
	int m, i, j;
	m = strlen(p);
	for(i=0; i<MAX_ASCII; i++)
		t[i]=m;
	for(j=0; j<m-1; j++)	
		t[p[j]] = m-1-j;
}

int horspool(char s[],char p[],int t[])
{
	int i, n, m, k;

	n = strlen(s);
	m = strlen(p);
	i = m-1;
	while(i<n) {
		k = 0;
		while((k<m)&&(p[m-1-k]==s[i-k]))	
			k++;

		if (k == m)	return i-m+1;
		else 	i = i+t[s[i]];
	}
	return -1;
}