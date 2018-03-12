// Apply FIRST for every production, not non-terminal...Include everything other than E. If E is found, also take FOLLOW of the head of this production
// The result of FOLLOW should be stored:
    // Use one array to store the terminals
    // and another array for the non-terminals whose FOLLOW should be taken.
    // Iterate through all non-terminals to create the final result [tuple of character, indication (0 for terminal, 1 for non-terminal)] for FOLLOW.
// Structure input as head [char] and body [tuple defined above]

#include <stdio.h>
#include <stdlib.h>

struct tuple
{
    int size; // following two char* should be of this size
    char* characters;
    char* flags; // 0 for terminal, 1 for non-terminal
};

struct production
{
    char head;
    struct tuple body;
};

struct production* grammar; // size: number of productions in the grammar
char** first; // size: number of non-terminals
char** follow; // size: number of non-terminals (Final result of FOLLOW | result of finalFOLLOW())
char** followT; // size: number of non-terminals (stores the terminals involved in the result of FOLLOW(A) | result of FOLLOW(char))
char** followN; // size: number of non-terminals (stores the non-terminals, whose FOLLOW should be included in the result of FOLLOW(A) | result of FOLLOW(char))
int* followResolved;
int N, nont = 0, t = 0; // N: number of productions | nont: number of non-terminals
int* firstE; // 1 if FIRST(A) has E | 0 if not | -1 if FIRST isn't calculated yet
char* terminals;
char* nonterminals;

void insert(char);
int terminalIndex(char);
int findIndex(char);
int findChar(char, int, int);
void FIRST(char);
void FOLLOW(char);
void finalFOLLOW(char);
void LL1();

int main()
{
    int i, n;
    printf("Number of productions: ");
    scanf("%d", &N);
    grammar = (struct production*)malloc(N*sizeof(struct production));
    nonterminals = (char*)malloc(N*sizeof(char)); // At most, there'll be the same number of non-terminals as there are productions
    terminals = (char*)malloc(50*sizeof(char));
    printf("Enter the grammar:\n");
    for(i = 0; i < N; i++)
    {
        printf("Head: ");
        scanf(" %c", &grammar[i].head);
        insert(grammar[i].head);
        // give the length without the terminal/non-terminal indication
        printf("Length of body: ");
        scanf("%d", &n);
        grammar[i].body.size = n;
        n = n+1;
        grammar[i].body.characters = (char*)malloc(n*sizeof(char));
        grammar[i].body.flags = (char*)malloc(n*sizeof(char));
        printf("Characters: ");
        scanf("%s", grammar[i].body.characters);
        printf("Flags: ");
        scanf("%s", grammar[i].body.flags);
    }
    printf("\n");
    firstE = (int*)malloc(nont*sizeof(int));
    first = (char**)malloc(nont*sizeof(char*));
    follow = (char**)malloc(nont*sizeof(char*));
    followT = (char**)malloc(nont*sizeof(char*));
    followN = (char**)malloc(nont*sizeof(char*));
    followResolved = (int*)malloc(nont*sizeof(int));
    for(i = 0; i < nont; i++)
    {
        firstE[i] = -1;
        first[i] = (char*)malloc(15*sizeof(char));
        follow[i] =(char*)malloc(30*sizeof(char));
        followT[i] = (char*)malloc(15*sizeof(char));
        followN[i] = (char*)malloc(15*sizeof(char));
        followResolved[i] = 0;
    }
    for(i = 0; i < nont; i++)
        FIRST(nonterminals[i]);
    for(i = 0; i < nont; i++)
        FOLLOW(nonterminals[i]);
    for(i = 0; i < nont; i++)
        finalFOLLOW(nonterminals[i]);

    // printing LL(1) table
    LL1();

    return 0;
}

// Adds head to the nonterminals array, if not already present, and increments nont
void insert(char head)
{
    int i;
    for(i = 0; i < nont; i++)
    {
        if(nonterminals[i] == head)
            return;
    }
    nonterminals[nont] = head;
    nont++;
    return;
}

int terminalIndex(char symbol)
{
    int i;
    for(i = 0; i < t; i++)
        if(symbol == terminals[i])
            return i;
    return -1;
}

// Takes head, and returns its index in the nonterminals array
int findIndex(char head)
{
    int i;
    for(i = 0; i < nont; i++)
        if(head == nonterminals[i])
            return i;
    return -1;
}

// Returns the index of character in the production at grammar[prod] if found, else returns -1. start indicates the index from where the search should start in the characters array of prod's body
int findChar(char character, int prod, int start)
{
    int i;
    for(i = start; i < grammar[prod].body.size; i++)
        if(character == grammar[prod].body.characters[i])
            return i;
    return -1;
}

void FIRST(char head)
{
    int i, j = 0, k, index1, index2;
    index1 = findIndex(head); // index1 holds the index of head
    for(i = 0; i < N; i++)
    {
        if(head == grammar[i].head)
        {
            if(grammar[i].body.flags[0] == '0') // first character in the body is a terminal
            {
                if(grammar[i].body.characters[0] == 'E')
                    firstE[index1] = 1;
                first[index1][j++] = grammar[i].body.characters[0];
            }
            else
            {
                index2 = findIndex(grammar[i].body.characters[0]); // index2 holds the index of the first non-terminal in the body of head's production
                FIRST(grammar[i].body.characters[0]);
                k = 0;
                while(first[index2][k] != '\0')
                {
                    first[index1][j] = first[index2][k];
                    j++;
                    k++;
                }
            }
        }
    }
    first[index1][j++] = '\0';
}

void FOLLOW(char head)
{
    int i, j = 0, k, l = 0, m, index1, index2, repeated, ind;
    index1 = findIndex(head);
    if(head == 'S')
    {
        followT[index1][j++] = '$';
    }
    // i tracks the grammar[]
    for(i = 0; i < N; i++)
    {
        ind = 0;
        while(1)
        {
            ind = findChar(head, i, ind);
            if(ind == -1)
                break;
            else
            {
                if(grammar[i].body.size > ind+1) // true if ind isn't the last character in grammar[i].body.characters[]
                {
                    if(grammar[i].body.flags[ind+1] == '0')
                    {
                        if(grammar[i].body.characters[ind+1] != 'E')
                            followT[index1][j++] = grammar[i].body.characters[ind+1];
                        else
                        {
                            if(head != grammar[i].head)
                            {
                                repeated = 0;
                                for(m = 0; m < l; m++)
                                    if(grammar[i].head == followN[index1][m])
                                    {
                                        repeated = 1;
                                        break;
                                    }
                                if(repeated == 0)
                                    followN[index1][l++] = grammar[i].head;
                            }
                        }
                    }
                    else
                    {
                        index2 = findIndex(grammar[i].body.characters[ind+1]);
                        if(firstE[index2] == 0)
                        {
                            k = 0;
                            while(first[index2][k] != '\0')
                            {
                                followT[index1][j] = first[index2][k];
                                j++;
                                k++;
                            }
                        }
                        else
                        {
                            k = 0;
                            while(first[index2][k] != '\0')
                            {
                                if(first[index2][k] != 'E')
                                {
                                    followT[index1][j] = first[index2][k];
                                    j++;
                                    k++;
                                }
                                else
                                    k++;
                            }
                            if(head != grammar[i].head)
                            {
                                repeated = 0;
                                for(m = 0; m < l; m++)
                                    if(grammar[i].head == followN[index1][m])
                                    {
                                        repeated = 1;
                                        break;
                                    }
                                if(repeated == 0)
                                    followN[index1][l++] = grammar[i].head;
                            }
                        }
                    }

                }
                else
                {
                    if(head != grammar[i].head)
                    {
                        repeated = 0;
                        for(m = 0; m < l; m++)
                            if(grammar[i].head == followN[index1][m])
                            {
                                repeated = 1;
                                break;
                            }
                        if(repeated == 0)
                            followN[index1][l++] = grammar[i].head;
                    }
                }
                ind++;
            }
        }
    }
    followT[index1][j] = '\0';
    followN[index1][l] = '\0';
}

// if FINAL of head is not resolved
//      for all non-terminals in followN[index][]
//          resolve the non-terminal if not already done
//          include its follow[] (i.e. follow[index2]) in follow[index1]
//      include followT[index1][i] if it isn't already present in follow[index1]
void finalFOLLOW(char head)
{
    int index1, index2;
    int i, j, k, l;

    index1 = findIndex(head);
    if(followResolved[index1] == 1)
        return;

    // i tracks follow[index1][]
    // j tracks followN[index1][]
    // k tracks follow[index2][] (i.e. follow[] of followN[index1][j])

    // j tracks followT[index1][]

    // l is used to iterate (for loops)

    i = 0;

    j = 0;
    while(followN[index1][j] != '\0')
    {
        index2 = findIndex(followN[index1][j]);
        if(followResolved[index2] == 0)
            finalFOLLOW(followN[index1][j]);

        k = 0;
        while(follow[index2][k] != '\0')
        {
            // insert follow[index2][] without any repetitions, into follow[index1][]
            for(l = 0; l < i; l++)
                if(follow[index2][k] == follow[index1][l])
                    break;
            if(l == i)
                follow[index1][i++] = follow[index2][k];
            k++;
        }
        j++;
    }

    j = 0;
    while(followT[index1][j] != '\0')
    {
        for(l = 0; l < i; l++)
            if(followT[index1][j] == follow[index1][l])
                break;
        if(l == i)
            follow[index1][i++] = followT[index1][j];
        j++;
    }
    follow[index1][i] = '\0';
    followResolved[index1] = 1;
}

void LL1()
{
    int i, j, k, l, index, tIndex;
    int*** ll = (int***)malloc(nont*sizeof(int**));
    char temp[30];

    // determine the terminals
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < grammar[i].body.size; j++)
        {
            if(grammar[i].body.flags[j]=='0' && grammar[i].body.characters[j]!='E')
            {
                for(k = 0; k < t; k++)
                    if(terminals[k] == grammar[i].body.characters[j])
                        break;
                if(k == t)
                    terminals[t++] = grammar[i].body.characters[j];
            }
        }
    }
    terminals[t++] = '$';

    //initializing ll[][]
    for(i = 0; i < nont; i++)
    {
        ll[i] = (int**)malloc(t*sizeof(int*));
        for(j = 0; j < t; j++)
        {
            ll[i][j] = (int*)malloc(6*sizeof(int));
            ll[i][j][0] = 0;
            for(k = 1; k < 6; k++)
                ll[i][j][k] = -1;
        }
    }

    // populating ll[][]
    for(i = 0; i < N; i++)
    {
        if(grammar[i].body.flags[0]=='0' && grammar[i].body.characters[0]!='E')
        {
            index = findIndex(grammar[i].head);
            tIndex = terminalIndex(grammar[i].body.characters[0]);
            j = ll[index][tIndex][0];
            ll[index][tIndex][++j] = i;
            ll[index][tIndex][0]++;
        }
        else if(grammar[i].body.flags[0]=='0' && grammar[i].body.characters[0]=='E')
        {
            j = 0;
            index = findIndex(grammar[i].head);
            while(follow[index][j] != '\0')
            {
                temp[j] = follow[index][j];
                j++;
            }
            index = findIndex(grammar[i].head);
            for(k = 0; k < j; k++)
            {
                tIndex = terminalIndex(temp[k]);
                l = ll[index][tIndex][0];
                ll[index][tIndex][++l] = i;
                ll[index][tIndex][0]++;
            }
        }
        else
        {
            j = 0;
            index = findIndex(grammar[i].body.characters[0]);
            while(first[index][j] != '\0')
            {
                if(first[index][j] != 'E')
                {
                    temp[j] = first[index][j];
                    j++;
                }
            }
            if(firstE[index] == 1)
            {
                index = findIndex(grammar[i].head);
                k = 0;
                while(follow[index][k] != '\0')
                {
                    for(l = 0; l < j; l++)
                        if(follow[index][k] == temp[l])
                            break;
                    if(l == j)
                        temp[j++] = follow[index][k];
                    k++;
                }
            }
            index = findIndex(grammar[i].head);
            for(k = 0; k < j; k++)
            {
                tIndex = terminalIndex(temp[k]);
                l = ll[index][tIndex][0];
                ll[index][tIndex][++l] = i;
                ll[index][tIndex][0]++;
            }
        }
    }

    // printing the result
    printf("\t");
    for(i = 0; i < t; i++)
        printf("\t%c\t", terminals[i]);
    printf("\n");
    for(i = 0; i < 99; i++)
        printf("_");
    printf("\n");
    for(i = 0; i < nont; i++)
    {
        printf("%c\t|", nonterminals[i]);
        for(j = 0; j < t; j++)
        {
            if(ll[i][j][0] == 0)
                printf("\t-\t");
            else
            {
                for(k = 1; k <= ll[i][j][0]; k++)
                {
                    l = ll[i][j][k];
                    if(k > 1)
                        printf(" | ");
                    printf("\t%c->%s\t", grammar[l].head, grammar[l].body.characters);
                }
            }
        }
        printf("\n");
    }
}