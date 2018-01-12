%{
%}
octal [0][0-7]+
hex [0][xX][0-9a-f]+|[0][xX][0-9A-F]+
%%
{octal} printf("%s is octal\n", yytext);
{hex} printf("%s is hexadecimal\n", yytext);
%%
int main()
{
        yyin = stdin;
        yylex();
}
int yywrap()
{
        return 1;
}

