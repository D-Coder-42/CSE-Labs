#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE *fptr;
	char filename[256], c;
	printf("Enter the filename to open for reading: \n");
	scanf("%s", filename);

	fptr = fopen(filename,"r");
	if (fptr == NULL) {
		fprintf(stderr, "Cannot open file %s\n", filename);
		exit(0);
	}

	int lines = 0, count = 0;
	c = fgetc(fptr);
	while (c != EOF) {
		if (c == '\n') lines++;
		count++;
		c = fgetc(fptr);
	}
	fclose(fptr);

	printf("The file %s has %d lines and %d characters.\n", filename, lines, count);
	return 0;
}