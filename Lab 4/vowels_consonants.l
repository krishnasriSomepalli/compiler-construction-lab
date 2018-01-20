%{
%}
vowel [aeiouAEIOU]
%%
{vowel} printf("%s: vowel\n", yytext);
[a-zA-Z] printf("%s: consonant\n", yytext);
%%
int main()
{
    yyin = stdin;
    yylex();
    return 0;
}
int yywrap()
{
    return 1;
}