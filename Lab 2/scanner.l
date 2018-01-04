%{
%}
character ([\'].[\'])
string ([\"].*[\"])
number ([1-9][0-9]*)|([0-9]*.[0-9]+)
identifier [a-zA-Z_][a-zA-Z0-9_]*
%%
{identifier} printf("%s is an identifier\n", yytext);
{number} printf("%s is a number\n", yytext);
{character} printf("%s is a character\n", yytext);
{string} printf("%s is a string\n", yytext);
"<<="|">>="|"++"|"--"|"<="|">="|"->"|"<<"|">>"|"&&"|"||"|"=="|"!="|"+="|"-="|"*="|"/="|"%="|"&="|"|="|"^="|"+"|"-"|"*"|"/"|"%"|"!"|"~"|"."|"<"|">"|"="|"&"|"|"|"^"|"?"|":" printf("%s is an operator\n", yytext);
"["|"]"|"("|")"|"{"|"}"|","|";"|"#"|"'"|"\""|"<"|">" printf("%s is a special character\n", yytext);
%%
int main(int argc, char** argv)
{
	yyin = stdin;
	yylex();
}
int yywrap()
{
	return 1;
}