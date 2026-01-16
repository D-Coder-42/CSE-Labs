#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void removePreprocessorDirs(FILE *src, FILE *dst) {
	int forward;
	long lexstart;
	char buf[256];
	int bufptr;

	lexstart = ftell(src);
	forward = fgetc(src);
	
	while (forward != EOF) {
		if (forward == '#') {
			bufptr = 0;
			forward = fgetc(src);

			while (forward != EOF && isalpha(forward)) {
				buf[bufptr++] = forward;
				forward = fgetc(src);
			}
			buf[bufptr] = '\0';
			if (strcmp(buf, "include") == 0 || strcmp(buf, "define") == 0) {
				while (forward != EOF && forward != '\n') {
					forward = fgetc(src);
				}
			} else {
				fseek(src, lexstart, SEEK_SET);
				forward = fgetc(src);
				while (forward != EOF && forward != '\n') {
					fputc(forward, dst);
					forward = fgetc(src);
				}
			}
			if (forward == '\n') {
				fputc('\n', dst);
				forward = fgetc(src);
			}
			lexstart = ftell(src)-1;
		} else {
			fputc(forward, dst);
			forward = fgetc(src);
			lexstart = ftell(src);
		}
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
	fdst = fopen("./files/output_q2.c","w+");

	removePreprocessorDirs(fsrc, fdst);

	fclose(fsrc);
	fclose(fdst);

	return 0;
}