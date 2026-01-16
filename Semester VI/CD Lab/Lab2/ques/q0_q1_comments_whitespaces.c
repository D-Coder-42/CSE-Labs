#include <stdio.h>
#include <stdlib.h>

void removeCommentsAndWhitespaces(FILE *src, FILE *dst) {
	char ca, cb;
	int inStringLiteral = 0;
	ca = fgetc(src);
	while (ca != EOF) {
		if (!inStringLiteral && (ca == ' ' || ca == '\t')) {
			fputc(' ', dst);
			while (ca == ' ' || ca == '\t') ca = fgetc(src);
		}
		if (ca == '/') {
			cb = fgetc(src);
			if (cb == '/') {
				while (ca != '\n') ca = fgetc(src);
				fputc(ca, dst);
			} else if (cb == '*') {
				do {
					while (ca != '*' && ca != EOF) ca = fgetc(src);
					if (ca == EOF) return;
					ca = fgetc(src);
				} while (ca != '/');
			} else {
				fputc(ca, dst);
				fputc(cb, dst);
			}
		}
		else fputc(ca, dst);
		ca = fgetc(src);
		if (ca == '"' || cb == '"') inStringLiteral ^= 1;
	}
}

int main(int argc, char const *argv[])
{
	FILE *fsrc, *fdst;

	fsrc = fopen("./files/input.c", "r");
	if (fsrc == NULL) {
		fprintf(stderr, "Cannot open file\n");
		exit(0);
	}
	fdst = fopen("./files/output_q0q1.c","w+");

	removeCommentsAndWhitespaces(fsrc, fdst);

	fclose(fsrc);
	fclose(fdst);

	return 0;
}