%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex();
	int yyerror(char *msg);
	extern FILE *yyin;
%}

%token NUMBER ID DT NL

%%
stat:	decl NL ;

decl:	DT idlist ';' decl |
		;

idlist:	ID |
		ID ',' idlist |
		ID '[' NUMBER ']' |
		ID '[' NUMBER ']' ',' idlist ;
%%

int yyerror(char *msg) {
	printf("Invalid expression\n");
	return 1;
}

void main() {
	printf("Enter the expression:\n");
	yyin=fopen("in.txt", "r");

	do {
		if (yyparse()) {
			printf("\nFailure\n");
			exit(0);
		} 
	} while (!feof(yyin));

	printf("Success\n");
}