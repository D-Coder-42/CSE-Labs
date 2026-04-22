%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "q1.tab.h"
%}

%%
[0-9]+ 		{ printf("%s\n", yytext); return NUMBER; }
"int"|"char"		{ printf("char\n"); return DT; }
[a-zA-Z]+ 	{ printf("%s\n", yytext); return ID; }

","         { printf(",\n"); return ','; }
";"         { printf(";\n"); return ';'; }
"["         { printf("[\n"); return '['; }
"]"         { printf("]\n"); return ']'; }

"$" 		{ return NL; }
[ \t\n]+	{}
%%

int yywrap() {
	return 1;
}