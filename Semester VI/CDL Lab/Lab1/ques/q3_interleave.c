#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE *rptr1, *rptr2, *wptr;
	char filename[256];
	int curfile = 0;

	printf("Enter the 1st filename: ");
	scanf("%s", filename);
	rptr1 = fopen(filename,"r");
	if (rptr1 == NULL) {
		fprintf(stderr, "Cannot open file %s\n", filename);
		exit(0);
	}

	printf("Enter the 2nd filename: ");
	scanf("%s", filename);
	rptr2 = fopen(filename,"r");
	if (rptr2 == NULL) {
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

	int c1, c2;
	c1 = fgetc(rptr1);
	c2 = fgetc(rptr2);

	while (c1 != EOF && c2 != EOF) {
		switch (curfile){
		case 0:
			if (c1 == '\n') curfile ^= 1;
			fputc(c1, wptr);
			c1 = fgetc(rptr1);
			break;
		case 1:
			if (c2 == '\n') curfile ^= 1;
			fputc(c2, wptr);
			c2 = fgetc(rptr2);
			break;
		}
	}

	if (c1 != EOF) {
		while (c2 != EOF) {
			fputc(c1, wptr);
			c1 = fgetc(rptr1);
		}
	}
	if (c2 != EOF) {
		while (c2 != EOF) {
			fputc(c2, wptr);
			c2 = fgetc(rptr2);
		}
	}

	fclose(rptr1);
	fclose(rptr2);

	printf("Written to file %s.\n", filename);
	return 0;
}