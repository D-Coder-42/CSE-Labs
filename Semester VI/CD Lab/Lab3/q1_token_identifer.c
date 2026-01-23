#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char token_name[16];
	char type[16];
	int index;
	unsigned int row,col;
} Token;

Token handleRelop(FILE *fp, char c, int* row, int* col) {
	Token t;
	char n = fgetc(fp);
	(*col)++;
	switch(c) {
	case '<':
		if (n == '=') strcpy(t.token_name, "<="); 
		else {
			strcpy(t.token_name, "<");
			ungetc(n, fp);
			(*col)--;
		}
		break;
	case '>':
		if (n == '=') strcpy(t.token_name, ">="); 
		else {
			strcpy(t.token_name, ">");
			ungetc(n, fp);
			(*col)--;
		}
		break;
	case '=':
		if (n == '=') strcpy(t.token_name, "=="); 
		else {
			strcpy(t.token_name, "=");
			ungetc(n, fp);
			(*col)--;
		}
		break;
	case '!':
		if (n == '=') strcpy(t.token_name, "!="); 
		else {
			strcpy(t.token_name, "!");
			ungetc(n, fp);
			(*col)--;
		}
		break;
	}
	t.row = *row;
	t.col = *col;
	return t;
}

Token handleArithmetic(FILE *fp, char c, int* row, int* col) {
	Token t;
	switch(c) {
	case '+': strcpy(t.token_name, "+"); break;
	case '-': strcpy(t.token_name, "-"); break;
	case '*': strcpy(t.token_name, "*"); break;
	case '/': strcpy(t.token_name, "/"); break;
	case '%': strcpy(t.token_name, "%"); break;
	}
	t.row = *row;
	t.col = *col;
	return t;
}

Token getNextToken(FILE *fp, int* row, int* col) {
	Token t;
	int c;
	int inStringLiteral = 0;

	while ((c = fgetc(fp)) != EOF) {
		(*col)++;

		if (c == '"') inStringLiteral ^= 1;
		if (inStringLiteral) continue;
		if (c == ' ' || c == '\t') continue;
		if (c == '\n') {
			(*row)++;
			*col = 1;
			printf("\n");
			continue;
		}

		/*if (isalpha(c)) {
			return handleIdentifier(fp,c,row,col);
		} else if (isdigit(c)) {
			return handleNumber(fp,c,row,col);
		} else*/ if (c == '<' || c == '>' || c == '=' || c == '!') {
			return handleRelop(fp,c,row,col);
		} else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
			return handleArithmetic(fp,c,row,col);
		} else {
			strcpy(t.token_name, "UNK");
			t.row = *row;
			t.col = *col;
			return t;
		}
	}

	strcpy(t.token_name, "EOF");
	return t;
}

int main(int argc, char const *argv[])
{
	FILE *fp = fopen("input.c", "r");
	if (fp == NULL) {
		perror("Cannot open file.\n");
		exit(0);
	}

	Token tk;
	unsigned int row = 1, col = 1;
	while (1) {
		tk = getNextToken(fp, &row, &col);
		if (strcmp(tk.token_name, "UNK") == 0) continue;
		if (strcmp(tk.token_name, "EOF") == 0) break;
		printf("<%s,%d,%d>", tk.token_name, tk.row, tk.col);
	}
 	
 	fclose(fp);
	return 0;
}