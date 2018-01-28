// Only grammars without left recursion
// 0: terminal
// 1: non-terminal
#include <stdio.h>
#include <stdlib.h>

struct production{
    char head;
    char* body;
};

struct production* grammar;

char* insert(char*, int*, char);
void FIRST(char, int);

int main()
{
    int N, n, nont = 0, i, j;
    char* nonTerminals;
    printf("Number of productions: ");
    scanf("%d", &N);
    grammar = (struct production*) malloc(N * sizeof(struct production));
    nonTerminals = (char*)malloc(N * sizeof(char)); // There can be atmost the same number of non-terminals as there are productions
    for(i = 0; i < N; i++)
    {
        printf("Head: ");
        scanf(" %c", &grammar[i].head);
        nonTerminals = insert(nonTerminals, &nont, grammar[i].head);
        printf("Length of body: ");
        // give the length without the terminal/non-terminal indication
        scanf("%d", &n);
        grammar[i].body = (char*)malloc((2*n+1)*sizeof(char));
        printf("Body: ");
        scanf("%s", grammar[i].body);
    }
    printf("\n");
    for(i = 0; i < nont; i++)
    {
        printf("FIRST(%c) = {", nonTerminals[i]);
        FIRST(nonTerminals[i], N);
        printf("}\n");
    }
    return 0;
}

char* insert(char* nonTerminals, int* nont, char head)
{
    int i;
    for(i = 0; i < *nont; i++)
    {
        if(nonTerminals[i] == head)
            return nonTerminals;
    }
    nonTerminals[*nont] = head;
    *nont = *nont + 1;
    return nonTerminals;
}

void FIRST(char head, int N)
{
    int i;
    for(i = 0; i < N; i++)
    {
        if(head == grammar[i].head)
        {
            if(grammar[i].body[1] == '0')
                printf(" %c ", grammar[i].body[0]);
            else
                FIRST(grammar[i].body[0], N);
        }
    }
}