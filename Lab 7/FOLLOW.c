// Only grammars without left recursion
// 0: terminal
// 1: non-terminal
// Use E to represent epsilon, $ for input end marker, S for start symbol
#include <stdio.h>
#include <stdlib.h>

struct production{
    char head;
    char* body;
};

struct production* grammar;
char* nonTerminals;
int* FIRSTflags; // set if FIRST(A) has E
int N, nont = 0;

void insert(char);
void FIRST(char, int, int);
void FOLLOW(char);

int main()
{
    int n, i, j;
    printf("Number of productions: ");
    scanf("%d", &N);
    grammar = (struct production*) malloc(N * sizeof(struct production));
    nonTerminals = (char*)malloc(N * sizeof(char)); // There can be atmost the same number of non-terminals as there are productions
    for(i = 0; i < N; i++)
    {
        printf("Head: ");
        scanf(" %c", &grammar[i].head);
        insert(grammar[i].head);
        printf("Length of body: ");
        // give the length without the terminal/non-terminal indication
        scanf("%d", &n);
        grammar[i].body = (char*)malloc((2*n+1)*sizeof(char));
        printf("Body: ");
        scanf("%s", grammar[i].body);
    }
    printf("\n");
    FIRSTflags = (int*)malloc(nont*sizeof(int));
    for(i = 0; i < nont; i++)
        FIRSTflags[i] = 0;
    for(i = 0; i < nont; i++)
    {
        printf("FIRST(%c) = {", nonTerminals[i]);
        FIRST(nonTerminals[i], 1, 1);
        printf("}\n");
    }
    for(i = 0; i < nont; i++)
    {
        printf("FOLLOW(%c) = {", nonTerminals[i]);
        FOLLOW(nonTerminals[i]);
        printf("}\n");
    }
    return 0;
}

void insert(char head)
{
    int i;
    for(i = 0; i < nont; i++)
    {
        if(nonTerminals[i] == head)
            return;
    }
    nonTerminals[nont] = head;
    nont++;
    return;
}

void FIRST(char head, int print, int epsilon)
{
    int i, j;
    for(i = 0; i < N; i++)
    {
        if(head == grammar[i].head)
        {
            if(grammar[i].body[1] == '0')
            {
                if(print == 1)
                {
                    if(grammar[i].body[0] == 'E')
                    {
                        if(epsilon == 1)
                           printf(" %c ", grammar[i].body[0]);
                    }
                    else
                        printf(" %c ", grammar[i].body[0]);
                }
                if(grammar[i].body[0] == 'E')
                {
                    for(j = 0; j < nont; j++)
                        if(head == nonTerminals[j])
                        {
                            FIRSTflags[j] = 1;
                            break;
                        }
                }
            }
            else
            {
                if(print == 0)
                    FIRST(grammar[i].body[0], 0, 0);
                else
                {
                    if(epsilon == 1)
                        FIRST(grammar[i].body[0], 1, 1);
                    else
                        FIRST(grammar[i].body[0], 1, 0);
                }
            }
        }
    }
}

// search for head in the body of all productions
// if the one on the immediate right is a terminal, DONE!
// if the one on the immediate right is a non-terminal, take its FIRST
// if either the one on the immediate right is Epsilon or if FIRST contains Epsilon, take the FOLLOW of the head of the production
// avoid possible recursions... (when will they occur?)
void FOLLOW(char head)
{
    int i, j, k, l = 0, m = 0, n, o, flag = 0;
    char temp1[10];
    char temp2[10];
    if(head == 'S')
        printf(" $ ");
    for(i = 0; i < N; i++)
    {
        j = 0;
        while(grammar[i].body[j] != '\0')
        {
            flag = 0;
            if(grammar[i].body[j] == head)
            {
                if(grammar[i].body[j+2] != '\0')
                {
                     if(grammar[i].body[j+3] == '0')
                     {
                        if(grammar[i].body[j+2] != 'E')
                        {
                            n = 1;
                            for(o = 0; o < l; o++)
                                if(grammar[i].body[j+2] == temp1[o])
                                {
                                    n = 0;
                                    break;
                                }
                            if(n == 1)
                            {
                                printf(" %c ", grammar[i].body[j+2]);
                                temp1[l++] = grammar[i].body[j+2];
                            }
                        }
                        else
                            if(grammar[i].head != head)
                                printf(" FOLLOW(%c) ", grammar[i].head);
                     }
                    else
                        // take FIRST
                        // ensure that it does not have E
                        // if it does, take FOLLOW of the head of this production
                        flag = 1;
                }
                else
                    if(grammar[i].head != head)
                        printf(" FOLLOW(%c) ", grammar[i].head);
            }
            if(flag == 1)
            {
                for(k = 0; k < nont; k++)
                {
                    if(nonTerminals[k] == grammar[i].body[j+2])
                    {
                        n = 1;
                        for(o = 0; o < m; o++)
                            if(nonTerminals[k] == temp2[o])
                            {
                                n = 0;
                                break;
                            }
                        if(FIRSTflags[k] == 0)
                        {
                            if(n == 1)
                            {
                                FIRST(nonTerminals[k], 1, 1);
                                temp2[m++] = nonTerminals[k];
                            }
                        }
                        else
                        {
                            if(n == 1)
                            {
                                FIRST(nonTerminals[k], 1, 0);
                                temp2[m++] = nonTerminals[k];
                            }
                            printf(" FOLLOW(%c) ", grammar[i].head);
                        }
                        break;
                    }
                }
                flag = 0;
            }
            j = j+2;
        }
    }
}