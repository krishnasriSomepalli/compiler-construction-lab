%{
        #include <stdio.h>
        void precision(char*);
%}
float [0-9]*.[0-9]+
number [1-9][0-9]*
%%
{float} {precision(yytext);}
{number} {printf("Precision: 0\n");}
%%
int main()
{
        yyin = stdin;
        yylex();
}
void precision(char* s)
{
        int i, j = 0;
        int count = 0;
        while(s[i] != '\0')
        {
                if(j == 1)
                        count++;
                if(s[i] == '.')
                        j = 1;
                i++;
        }
        printf("Precision: %d\n", count);
}
int yywrap()
{
        return 1;
}
