%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	
	int yylex();
	int yyerror(char *msg);
%}

%token NUM

%%
input:	
		| input line
		;

line:	'$'
		| exp '$'       { printf("Result: %d\n", $1); }
		;

exp:	NUM             { $$ = $1; }
		| exp exp '+'   { $$ = $1 + $2; }
		| exp exp '-'   { $$ = $1 - $2; }
		| exp exp '*'   { $$ = $1 * $2; }
		| exp exp '/'   { 
                            if ($2 == 0) {
                                yyerror("Division by zero");
                                $$ = 0; 
                            } else {
                                $$ = $1 / $2; 
                            }
                        }
		| exp exp '^'   { $$ = pow($1, $2); }
		| exp 'n'       { $$ = -$1; }
		;
%%

int yyerror(char *msg) {
	printf("Error: %s\n", msg);
	return 1;
}

int main() {
	printf("Enter a postfix expression ending with '$':\n");
	yyparse();
	return 0;
}