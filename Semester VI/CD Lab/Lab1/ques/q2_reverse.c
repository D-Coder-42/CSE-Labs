#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE *rptr, *wptr;
	char filename[256];
	int c;

	printf("Enter the filename to open for reading: ");
	scanf("%s", filename);
	rptr = fopen(filename,"r");
	if (rptr == NULL) {
		fprintf(stderr, "Cannot open file %s\n", filename);
		exit(0);
	}

	printf("Enter the filename to open for writing: ");
	scanf("%s", filename);
	wptr = fopen(filename,"w+");
	if (wptr == NULL) {
		fprintf(stderr, "Cannot open file %s\n", filename);
		exit(0);
	}

	int size = 0, i;
	fseek(rptr, 0, SEEK_END);
	size = ftell(rptr);

	printf("Size of file: %d\n", size);
	for (i = size-1; i >= 0; i--) {
		fseek(rptr, i, SEEK_SET);
		c = fgetc(rptr);
		fputc(c, wptr);
	}
	fclose(rptr);
	fclose(wptr);

	printf("File copied in reverse to %s.\n", filename);
	return 0;
}