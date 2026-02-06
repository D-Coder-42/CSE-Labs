%{
int vowels = 0;
int cons = 0;
%}

%%
[aeiouAEIUO]		{vowels++;}
[a-zA-Z]			{cons++;}
%%

int main() {
	yylex();
	printf("%d, %d\n", vowels, cons);
}

int yywrap() {
	return 1;
}