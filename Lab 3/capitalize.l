%{
        #include <stdio.h>
        #include <ctype.h>
        int k = 0;
        void capitalize(char*);
%}
comment "//"
lowercase [a-z]
%%
{comment} {k = 1; printf("//");}
{lowercase} {if(k == 1) capitalize(yytext);}
[\n] {k = 0; printf("\n");}
%%
int main()
{
        yyin = stdin;
        yylex();
}
void capitalize(char* s)
{
        printf("%c", toupper(s[0]));
}
int yywrap()
{
        return 1;
}