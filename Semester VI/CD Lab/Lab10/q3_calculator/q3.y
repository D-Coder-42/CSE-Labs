%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex();
	int yyerror(char *msg);
	extern FILE *yyin;
%}

%token NUMBER NL

%%
program:    expr NL { printf("Result: %d\n", $1); exit(0); } ;

expr:	expr '+' term   { $$ = $1 + $3; } |
		expr '-' term   { $$ = $1 - $3; } |
		term            { $$ = $1; } ;

term:	term '*' factor { $$ = $1 * $3; } | 
		term '/' factor { 
                            if ($3 == 0) {
                                yyerror("Divide by Zero Error");
                                exit(1);
                            }
                            $$ = $1 / $3; 
                        } |
		factor          { $$ = $1; } ;

factor:	'(' expr ')'    { $$ = $2; } |
		NUMBER          { $$ = $1; } ;
%%

int yyerror(char *msg) {
	printf("Error: %s\n", msg);
	return 1;
}

void main() {
	printf("Enter the expression:\n");
	yyin = fopen("in.txt", "r");

	if (!yyin) {
		printf("Error: Could not open in.txt\n");
		exit(1);
	}

	do {
		if (yyparse()) {
			printf("\nFailure\n");
			exit(0);
		} 
	} while (!feof(yyin));

	printf("Success\n");
}