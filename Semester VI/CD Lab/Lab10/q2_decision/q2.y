%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex();
	int yyerror(char *msg);
	extern FILE *yyin;
%}

%token IF THEN ELSE OTHER NUMBER ID
%token OR AND RELOP ADDOP MULOP LPAREN RPAREN NL

%%
program:	stmt_list ;

stmt_list:	stmt NL stmt_list |
			;

stmt:		matched_stmt | 
			open_stmt ;

matched_stmt: IF expr THEN matched_stmt ELSE matched_stmt | 
			OTHER ;

open_stmt:	IF expr THEN stmt | 
			IF expr THEN matched_stmt ELSE open_stmt ;

expr:		expr OR b_term | 
			b_term ;

b_term:		b_term AND b_factor | 
			b_factor ;

b_factor:	arith_expr RELOP arith_expr | 
			arith_expr ;

arith_expr:	arith_expr ADDOP term | 
			term ;

term:		term MULOP factor | 
			factor ;

factor:		'(' expr ')' | 
			ID | 
			NUMBER ;
%%

int yyerror(char *msg) {
	printf("Invalid expression\n");
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