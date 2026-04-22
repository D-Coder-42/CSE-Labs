%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "q4.tab.h"
%}

%%
[0-9]+      { 
                yylval = atoi(yytext); 
                return NUM; 
            }

[ \t\n]+    { }

"$"         { return '$'; }

.           { return yytext[0]; }
%%

int yywrap() {
	return 1;
}