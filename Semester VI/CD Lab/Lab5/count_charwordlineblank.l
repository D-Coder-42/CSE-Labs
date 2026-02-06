%{
int chars = 0;
int words = 0;
int lines = 0;
int blanks = 0;
%}

%%
[a-zA-Z]+	{words++; chars += yyleng;}
\n 			{chars++; lines++;}
" "			{blanks++; chars++;}
.			{chars++;}
%%

int main() {
	yylex();
	printf("%d, %d, %d. %d\n", lines, words, chars, blanks);
}

int yywrap() {
	return 1;
}