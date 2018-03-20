#include<iostream>
#include<string.h>
using namespace std;
string input;
char op[10], arg1[10], arg2[10];
int j = 0, l;
int rep = 1;

void checkUnaryMinus(int);
int getPrecedence(int);
void changeRepresentation(int);
void printCode();

int main()
{
        cout << "Enter any expression: ";
        cin >> input;
        l = input.length();
        for(int i = 0; i < l; i++)
                if(input[i] == '-')
                        checkUnaryMinus(i);
        for(int n = 6; n >= 4; n--)
        {
                for(int i = 0; i < l; i++)
                {
                        int prec = getPrecedence(i);
                        if(prec == n)
                        {
                                changeRepresentation(i);
                                i = 0;
                        }
                }
        }
        cout << "\nThree Address Code:\n";
        for(int i = 0; i < j; i++)
                cout << i+1 << "\t" << op[i] << "\t" << arg1[i] << "\t" << arg2[i] << endl;
        printCode();
        return 0;
}

void checkUnaryMinus(int i)
{
        if(!isalpha(input[i]))
        {
                op[j] = 'm';
                arg1[j] = input[i+1];
                arg2[j] = ' ';
                input.replace(i-1, 2, " ");
                input[i] = rep+'0';
                rep++;
                l = l-1;
                j++;
        }
}

int getPrecedence(int i)
{
        char op = input[i];
        if(op == '*' || op == '/')
                return 6;
        else if(op == '+' || op == '-')
                return 5;
        else if(op == '=')
                return 4;
        else
                return -1;
}

void changeRepresentation(int i)
{
        op[j] = input[i];
        arg1[j] = input[i-1];
        arg2[j] = input[i+1];
        input.replace(i-1, 3, " ");
        input[i-1] = rep+'0';
        rep++;
        l = l - 2;
        j++;
}

void printCode()
{
        cout << "\n\nOutput of Code Generation:\n";
        for(int i = 0; i < j; i++)
        {
                if(op[i] != '=')
 {
                        cout << "MOV " << arg1[i] << ", R\n";
                        if(op[i] == '+')
                                cout << "SUM ";
                        else if(op[i] == '-')
                                cout << "DIF ";
                        else if(op[i] == '*')
                                cout << "MUL ";
                        else if(op[i] == '/')
                                cout << "DIV ";
                        if(op[i] != 'm')
                                cout << arg2[i] << ", R\n";
                        else
                                cout << "MUL -1, R\n";
                }
                else
                        cout << "MOV " << arg1[i] << ", " << arg2[i] << endl;
        }
}