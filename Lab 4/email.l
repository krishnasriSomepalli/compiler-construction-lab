%{
    // Rules not included:
    // Each subdomain should be {1,63}.
    // The length should be {6,254} in total.
    // Other special characters that are normally approved of haven't been included here
%}
id ^[a-zA-Z0-9][a-zA-Z0-9._%+\-]{0,63}"@"(([a-zA-Z0-9]([\-]*[a-zA-Z0-9])+)"."){1,125}[a-zA-Z]{2,63}$
%%
{id} printf("Valid email ID\n");
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