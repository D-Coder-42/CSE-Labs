#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *keywords[] = {"auto","double","int","struct","break","else","long","switch","case","enum", "register","typedef","char","extern","return","union","const","float","short", "unsigned","continue","for","signed","void","default","goto","sizeof","volatile", "do","if","static","while" }; 
const int keywordcount = sizeof(keywords)/sizeof(keywords[0]);

int isKeyword(const char *buf) { 
	for (int i = 0; i < keywordcount; i++) { 
		if (strcmp(buf, keywords[i]) == 0) 
			return 1; 
	} 
	return 0; 
}

void printCaps(char *str, int n) {
	for (int i = 0; i < n; i++) {
		if (str[i] >= 'a' && str[i] <= 'z')
			printf("%c", str[i]-32);
		else
			printf("%c", str[i]);
	}
	printf("\n");
}

void printKeywords(FILE *src) {
	int forward;
	char buf[256];
	int bufptr;

	forward = fgetc(src);
	while (forward != EOF) {
		if (isalpha(forward)) {
			bufptr = 0;
			while (forward != EOF && isalpha(forward)) {
				buf[bufptr++] = forward;
				forward = fgetc(src);
			}
			buf[bufptr] = '\0';
			if (isKeyword(buf)) printCaps(buf, strlen(buf));
		} else 
			forward = fgetc(src);
	}
}


int main(int argc, char const *argv[])
{
	FILE *fsrc;

	fsrc = fopen("./files/input.c", "r");
	if (fsrc == NULL) {
		fprintf(stderr, "Cannot open file\n");
		exit(0);
	}

	printKeywords(fsrc);

	fclose(fsrc);

	return 0;
}
