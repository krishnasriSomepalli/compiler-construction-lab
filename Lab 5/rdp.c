// Recursive Descent Parser (backtracking) in C for the following grammar:
// (Converting to BNF form???)
// (What if input doesn't follow the grammar?)
// S -> E
// E -> T | E + T | E - T
// T -> F | T * F | T / F
// F -> a | b | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

// Here 'e' denotes epsilon, i.e. empty string

// We need to eliminate left recursion and make the grammar left-factored:
// E -> TE'
// E' -> +TE' | -TE' | e
// T -> FT'
// T' -> *FT' | /FT' | e
// F -> a | b | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
#include <stdio.h>
#include <string.h>

char peek();
char getValue();

int parseE();
int parseEPrime();
int parseT();
int parseTPrime();
int parseF();

int i = 0;
char input[30];

int main()
{
    printf("Input string: ");
    scanf("%s", input);
    if(parseE())
        printf("Accepted.\n");
    else
        printf("Rejected.\n");
    return 0;
}

char peek()
{
    return input[i];
}

char getValue()
{
    return input[i++];
}

int parseE()
{
    if(parseT())
        if(parseEPrime())
            return 1;
    return 0;
}

int parseEPrime()
{
    char token = peek();
    if(token == 'e')
    {
        getValue();
        return 1;
    }
    else
    {
        if(token != '+' || token != '-')
        {
            return 1;
        }
        else
        {
            getValue();
            if(parseT())
                if(parseEPrime())
                    return 1;
        }
    }
    return 0;
}

int parseT()
{
    if(parseF())
        if(parseTPrime())
            return 1;
    return 0;
}

int parseTPrime()
{
    char token = peek();
    if(token == 'e')
    {
        getValue();
        return 1;
    }
    else
    {
        if(token != '*' || token != '/')
        {
            return 1;
        }
        else
        {
            getValue();
            if(parseF())
                if(parseTPrime())
                    return 1;
        }
    }
    return 0;
}

int parseF()
{
    char token = peek();
    if(token == 'a' || token == 'b' || token == '0' || token == '1' || token == '2' || token == '3' || token == '4' || token == '5' || token == '6' || token == '7' || token == '8' || token == '9')
    {
        getValue();
        return 1;
    }
    else
        return 0;
}