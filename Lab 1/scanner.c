// token names: invalid, keyword, identifier, number, string, operator, special
// Scanner is greedy
// ambiguity: *, &, <, >
// Should a scanner be able to resolve the ambiguity based on the context?

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
char* keywords[] = {"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"};
char* operators_3[] = {"<<=",">>="};
char* operators_2[] = {"++","--","<=",">=","->","<<",">>","&&","||","==","!=","+=","-=","*=","/=","%=","&=","|=","^="};
char operators_1[] = {'+','-','*','/','%','!','~','.','<','>','=','&','|','^','?',':'};
char* special[] = {"[","]","(",")","{","}",",",";","#","'","\"","<",">"};
int check_k(char*);
int check_s(char);
int check_op2(char*);
int check_op1(char);
int main(int argc, char** argv)
{
    FILE* fp;
    int c, len = 0, flag = 0, opflag = 1, i;
    char ch;
    char lexeme[20];
    if(argc != 2)
    {
        printf("Usage: ./a.out <input file name>\n");
        exit(0);
    }
    fp = fopen(argv[1], "rb");
    printf("LEXEME\t\t\t\t\tTOKEN NAME\n");
    for(i=0; i<55; i++)
        printf("-");
    printf("\n");
    while((c = fgetc(fp)) != EOF)
    {
        ch = (char)c;
        //identifying a keyword/identifier
        if(ch == '_' || (isalpha(c) != 0))
        {
            len = 0;
            lexeme[len++] = ch;
            while((c = fgetc(fp)) != EOF)
            {
                ch = (char)c;
                if((ch != '_') && (isalnum(c) == 0))
                {
                    lexeme[len] = '\0';
                    if(check_k(lexeme) == 0)
                        printf("%s\t\t\t\t\tkeyword\n", lexeme);
                    else
                        printf("%s\t\t\t\t\tidentifier\n", lexeme);
                     // set fp 1 place back
                    fseek(fp, -1L, SEEK_CUR);
                    break;
                }
                else
                    lexeme[len++] = ch;
            }
            if(c == EOF)
                // set fp 1 place back
                fseek(fp, -1L, SEEK_CUR);
        }
        // ignore any comments
        else if(ch == '/')
        {
            if((c == fgetc(fp)) != EOF)
            {
                ch = (char)c;
                if(ch == '/')
                {
                    while((c = fgetc(fp)) != EOF && (c != 10));
                    if(c == EOF)
                        // set fp 1 place back
                        fseek(fp, -1L, SEEK_CUR);
                }
                else if(ch == '*')
                {
                    while(1)
                    {
                        while((c = fgetc(fp)) != EOF && (c != 42));
                        if(c != EOF)
                            if((c == fgetc(fp)) != EOF)
                                if(c == 47)
                                    break;
                            else
                                fseek(fp, -1L, SEEK_CUR);
                        else
                            // set fp 1 place back
                            fseek(fp, -1L, SEEK_CUR);
                    }
                }
                else
                {
                    // operator checking
                    fseek(fp, -2L, SEEK_CUR);
                }
            }
            else
                // set fp 1 place back
                fseek(fp, -1L, SEEK_CUR);
        }
        // identifying a string literal
        else if(ch == '\"')
        {
            len = 0;
            lexeme[len++] = ch;
            while(((c = fgetc(fp)) != EOF) && (c != 34) && (c != 10))
            {
                ch = (char)c;
                lexeme[len++] = ch;
            }
            if(c == 34)
                lexeme[len++] = (char)c;
            lexeme[len] = '\0';
            if(c == 34)
                printf("%s\t\t\t\t\tstring\n", lexeme);
            else if(c == 10 || c == EOF)
                printf("%s\t\t\t\t\tinvalid\n", lexeme);
            if(c == EOF)
                // set fp 1 place back
                fseek(fp, -1L, SEEK_CUR);
        }
        // identifying a character literal
        else if(ch == '\'')
        {
            if((c = fgetc(fp)) != EOF)
            {
                len = 0;
                lexeme[len++] = ch;
                ch = (char)c;
                lexeme[len++] = ch;
                if((c = fgetc(fp)) != EOF)
                {
                    if(c == 39)
                    {
                        ch = (char)c;
                        lexeme[len++] = ch;
                        lexeme[len] = '\0';
                        printf("%s\t\t\t\t\tcharacter\n", lexeme);
                    }
                    else // read the invalid lexeme until ' or \n is encountered
                    {
                        while(((c = fgetc(fp)) != EOF) && (c != 39) && (c != 10))
                        {
                            ch = (char)c;
                            lexeme[len++] = ch;
                        }
                        if(c == 39)
                            lexeme[len++] = (char)c;
                        lexeme[len] = '\0';
                        printf("%s\t\t\t\t\tinvalid\n", lexeme);
                        if(c == EOF)
                            // set fp 1 place back
                            fseek(fp, -1L, SEEK_CUR);
                    }
                }
                else
                    // set fp 1 place back
                    fseek(fp, -1L, SEEK_CUR);
            }
            else
                // set fp 1 place back
                fseek(fp, -1L, SEEK_CUR);
        }
        // identifying a three character operator
        else if((ch == '<' || ch == '>') && (opflag == 1))
        {
            if((c = fgetc(fp)) != EOF)
            {
                len = 0;
                lexeme[len++] = ch;
                if((ch == '<' && c == 60) || (ch == '>' && c == 62))
                {
                    ch = (char) c;
                    lexeme[len++] = ch;
                    if((c = fgetc(fp)) != EOF && c == 61)
                    {
                        ch = (char)c;
                        lexeme[len++] = ch;
                        lexeme[len] = '\0';
                        printf("%s\t\t\t\t\toperator\n", lexeme);
                    }
                    else
                    {
                        // set fp 3 places back - operator checking / EOF
                        fseek(fp, -3L, SEEK_CUR);
                        opflag = 2;
                    }
                }
                else
                {
                    // set fp 2 places back - operator checking
                    fseek(fp, -2L, SEEK_CUR);
                    opflag = 2;
                }
            }
            else
            {
                // set fp 2 places back
                fseek(fp, -2L, SEEK_CUR);
                opflag = 3;
            }
        }
        // identifying a two character operator
        else if((ch == '+' || ch == '-' || ch == '&' || ch == '|' || ch == '=' || ch == '*' || ch == '/' || ch == '%' || ch == '^' || ch == '!') || (opflag == 2 && (ch == '<' || ch == '>')))
        {

            if((c = fgetc(fp)) != EOF)
            {
                len = 0;
                lexeme[len++] = ch;
                ch = (char)c;
                lexeme[len++] = ch;
                lexeme[len] = '\0';
                if(check_op2(lexeme) == 0)
                {
                    printf("%s\t\t\t\t\toperator\n", lexeme);
                    opflag = 1;
                }
                else
                // set fp 2 place back - operator checking
                {
                    fseek(fp, -2L, SEEK_CUR);
                    opflag = 3;
                }
            }
            else
            {
                // set fp 2 places back
                fseek(fp, -2L, SEEK_CUR);
                opflag = 3;
            }
        }
        // identifying a single character operator
        else if(!check_op1(ch))
        {
            printf("%c\t\t\t\t\toperator\n", ch);
            opflag = 1;
        }
        //identifying a special symbol
        else if(!check_s(ch))
            printf("%c\t\t\t\t\tspecial symbol\n", ch);
        // identifying a number
        else if(isdigit(ch) != 0)
        {
            flag = 0;
            len = 0;
            lexeme[len++] = ch;
            while(((c = fgetc(fp)) != EOF) && ((isdigit(c) != 0) || (c == 46)))
            {
                if(c == 46 && flag == 1)
                {
                    flag = 1;
                    ch = (char)c;
                    lexeme[len++] = ch;
                }
                else
                {
                    flag = 0;
                    ch = (char)c;
                    lexeme[len++] = ch;
                }
            }
            lexeme[len] = '\0';
            if(lexeme[len-1] == '.')
                flag = 1;
            if(lexeme[0] == '.')
                flag = 1;
            if(flag == 1)
                printf("%s\t\t\t\t\tinvalid\n", lexeme);
            else
                printf("%s\t\t\t\t\tnumber\n", lexeme);
            // set fp 1 place back
            if((c == EOF) || ((isdigit(c) == 0) && (c != 46)))
                fseek(fp, -1L, SEEK_CUR);
        }
    }
    fclose(fp);
    return 0;
}

// returns 0 if the argument belongs to the defined keywords set
int check_k(char* lexeme)
{
    int i;
    for(i = 0; i < 32; i++)
        if(strcmp(lexeme, keywords[i]) == 0)
            return 0;
    return -1;
}

// returns 0 if the argument belongs to the defined special set
int check_s(char ch)
{
    int i;
    char s[2];
    s[0] = ch;
    s[1] = '\0';
    for(i = 0; i < 13; i++)
        if(strcmp(s, special[i]) == 0)
            return 0;
    return -1;
}

// returns 0 if the argument belongs to the defined operators_2 set
int check_op2(char* lexeme)
{
    int i;
    for(i = 0; i < 19; i++)
        if(strcmp(lexeme, operators_2[i]) == 0)
            return 0;
    return -1;
}

// returns 0 if the argument belongs to the defined operators_1 set
int check_op1(char ch)
{
    int i;
    for(i = 0; i < 16; i++)
        if(ch == operators_1[i])
            return 0;
    return -1;
}