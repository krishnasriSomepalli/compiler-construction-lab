#include <stdio.h>
#include <stdlib.h>
int main()
{
    char file[20], input;
    int i, chars = 0, spaces = 0, tabs = 0, lines = 0;
    printf("File: ");
    scanf("%s", file);
    FILE* fp = fopen(file, "r");
    if(fp == NULL)
    {
        printf("Error in opening the file\n");
        exit(1);
    }
    while((input = (char)fgetc(fp)) != (char)EOF)
    {
        chars++;
        if(input == ' ')
            spaces++;
        else if(input == '\t')
            tabs++;
        else if(input == '\n')
            lines++;
    }
    if(fclose(fp) != 0)
    {
        printf("Error in closing the file\n");
        exit(2);
    }
    printf("Characters: %d\nSpaces: %d\nTabs: %d\nLines: %d\n", chars, spaces, tabs, lines);
    return 0;
}