%{
%}
infix ([A-Za-z]+[+\-*/][A-Za-z]+) | ([A-Za-z]+" "[+\-*/]" "[A-Za-z]+)
prefix ([+\-*/][A-Za-z]+" "[A-Za-z]+) | ([+\-*/]" "[A-Za-z]+" "[A-Za-z]+)
postfix ([A-Za-z]+" "[A-Za-z]+[+\-*/]) | ([A-Za-z]+" "[A-Za-z]+" "[+\-*/])
%%
{infix} printf("%s: infix expression\n", yytext);
{prefix} printf("%s: prefix expression\n", yytext);
{postfix} printf("%s: postfix expression\n", yytext);
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