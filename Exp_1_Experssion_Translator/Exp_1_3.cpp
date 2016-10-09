/*
Function:   Read a in-fix expression as input,
            then transform it into a post-fix
            expression.
Author:     22920142203831 / Xiamen Univ 2016.
Input:      Legal in-fix expression containing
            constants, variables, '+', '-', '*',
            '/' and parenthesis. Variables start
            with a letter and contain only
            letters and digits.
            E.g. 5 + ((31 * t1) - 5) / (6 - 1)
Output:     Corresponding post-fix expression
            containing constants, variables,
            '+', '-', '*' and '/'.
Original Grammar:
            expr    --> expr + term    {print('+')}
                     |  expr - term    {print('-')}
                     |  term
            term    --> term * factor  {print('*')}
                     |  term / factor  {print('/')}
                     |  factor
            factor  --> ( expr )
                     |  id              {print(lexeme)}
                     |  num             {print(tokenVal)}

Translation Scheme with Left-recursive Eliminated:
            expr    --> term rest
            rest    --> + term {print'+'} rest
                     |  - term {print'-'} rest
                     |  EMPTY
            term    --> factor section
            section --> * factor {print'*'} section
                     |  / factor {print'/'} section
                     |  EMPTY
            factor  --> ( expr )
                     |  id              {print(lexeme)}
                     |  num             {print(tokenVal)}
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _TKN_NUM 500
#define _TKN_ID  600

int lookAhead, tokenVal;
char lexeme[1024];

int GetToken()
{
    int now, i;
    while(1){
        now = getchar();
        if((now == ' ') || (now == '\t') ) {
            ;
        }
        else if(isdigit(now)) {
                tokenVal = 0;
            do{
                tokenVal = tokenVal * 10 + now - '0';
                now = getchar();
            }while(isdigit(now));
            ungetc(now, stdin);
            return _TKN_NUM;
        }
        else if(isalpha(now)) {
            i=0;
            do{
                lexeme[i] = now;
                i++;
                now = getchar();
            }while(isalnum(now));
            lexeme[i] = '\0'; //Set end sign for string
            ungetc(now, stdin);
            return _TKN_ID;
        }
        else{
            tokenVal = 0;
            return now;
        }
    }
}

void Match(int now)
{
    if(lookAhead = now){
        lookAhead = GetToken();
    }else{
        printf("Expression illegal.(LookAhead: %c)\n",lookAhead);
        exit(1); //Illegal input
    }
}
void Expr();

void Factor()
{
    if(lookAhead == _TKN_NUM){
        printf("%d ",tokenVal); Match(lookAhead);
    }
    else if(lookAhead == _TKN_ID){
        printf("%s ",lexeme); Match(lookAhead);
    }
    else if(lookAhead == '('){
        Match('('); Expr(); Match(')');
    }
    else{
        printf("Match Error.(LookAhead: %c)\n", lookAhead);
        exit(1); //Illegal input
    }
}

void Section()
{
    switch(lookAhead){
    case '*':
        Match('*'); Factor(); printf("* "); Section();
        break;
    case '/':
        Match('/'); Factor(); printf("/ "); Section();
        break;
    default: //Empty Factor
        break;
    }
}

void Term()
{
    Factor();
    Section();
}

void Rest()
{
    switch(lookAhead){
    case '+':
        Match('+'); Term(); printf("+ "); Rest();
        break;
    case '-':
        Match('-'); Term(); printf("- "); Rest();
        break;
    default: //Empty Rest
        break;
    }
}

void Expr()
{
    Term();
    Rest();
}

int main(int argc, char *argv[])
{
    for(int c = 0; c < argc ; c++){
        printf("Input in-fix expression:");
        lookAhead = GetToken();

        printf("Translated post-fix expression:");
        Expr();

        if(lookAhead != '\n')
        {
            printf("illegal input.(lookAhead: %c)\n", lookAhead);
            exit(1); //Illegal input
        }

        printf("\nTranslated successfully.\n");
    }
    exit(0); //Normally exit
}
