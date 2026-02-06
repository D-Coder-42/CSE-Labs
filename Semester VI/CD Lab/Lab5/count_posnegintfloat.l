%{
int chars = 0;
int words = 0;
int lines = 0;
%}

%%
[+]?[1-9][0-9]*
-[1-9][0-9]*
[+]?[0-9]+"."[0-9]+
-[0-9]+"."[0-9]+
%%

int main() {
	yylex();
	printf("%d, %d, %d\n", lines, words, chars);
}

int yywrap() {
	return 1;
}