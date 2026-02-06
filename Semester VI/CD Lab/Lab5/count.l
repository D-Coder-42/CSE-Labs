%{
int chars = 0;
int words = 0;
int lines = 0;
%}

%%
[a-zA-Z]+	{words++; chars += yyleng;}
\n 			{chars++; lines++;}
.			{chars++;}
%%

int main() {
	yylex();
	printf("%d, %d, %d\n", lines, words, chars);
}

int yywrap() {
	return 1;
}