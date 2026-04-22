%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex();
	int yyerror();
	extern FILE *yyin;
%}

%token NUMBER ID  PLUS MINUS NL
%left '+' '-'

%%
stat:	exp NL ;

exp:	exp PLUS term |
		exp MINUS term |
		term ;

term:	ID |
		NUMBER ;
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