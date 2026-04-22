%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "q2.tab.h" /* Make sure this matches your YACC output header name */
%}

%%
"if"			{ printf("if\n"); return IF; }
"then"			{ printf("then\n"); return THEN; }
"else"			{ printf("else\n"); return ELSE; }
"other"			{ printf("other\n"); return OTHER; }

"||"|"or"		{ printf("OR\n"); return OR; }
"&&"|"and"		{ printf("AND\n"); return AND; }
"=="|"!="|"<"|">"|"<="|">=" { printf("%s\n", yytext); return RELOP; }
"+"|"-"			{ printf("%s\n", yytext); return ADDOP; }
"*"|"/"			{ printf("%s\n", yytext); return MULOP; }

"("				{ printf("(\n"); return '('; }
")"				{ printf(")\n"); return ')'; }

[0-9]+ 			{ printf("%s\n", yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { printf("%s\n", yytext); return ID; }

"$" 			{ return NL; }
[ \t\n]+		{}
%%

int yywrap() {
	return 1;
}