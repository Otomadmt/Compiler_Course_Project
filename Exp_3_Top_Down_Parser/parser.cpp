#include "lex.yy.c"

static int  lookAhead;
static unsigned int status = 0;

void Block();
void Stmt();
void Expr();

void
match(int tokenID) {
    if (lookAhead == tokenID) {
        lookAhead = yylex();
    } else {
        printf("match error\n[INFO] lookAhead %d %s but excepting %d\n", 
                lookAhead, yytext, tokenID); 
        status++;
    }
}

void
Factor() {
    switch(lookAhead) {
        case LEFT_BRACKET:
            match(LEFT_BRACKET);Expr();match(RIGHT_BRACKET);
            break;
        case ID_TKN:
            match(ID_TKN);
            break;
        case NUM_TKN:
            match(NUM_TKN);
            break;
        default:
            printf("Factor error (lookAhead:%d %s)\n", lookAhead, yytext); status++;
    }
}

void
Section() {
    switch(lookAhead) {
        case OP_MUDI_TKN:
            match(OP_MUDI_TKN);Factor();Section();
            break;
        default:
            break;
    }
}
 
void
Term() {
    Factor();
    Section();
}

void
Rest() {
    switch(lookAhead) {
        case OP_ADMI_TKN:
            match(OP_ADMI_TKN);Term();Rest();
            break;
        default:
            break;
    }
}

void
Expr() {
    Term();
    Rest();
}

void
Comp() {
    switch(lookAhead) {
        case RELOP_TKN:
            match(RELOP_TKN);Expr();
            break;
        default:
            break;
    }
}

void
Bool() {
    Expr();
    Comp();
}

void
Elsepart() {
    switch(lookAhead) {
        case ELSE_TKN:
            match(ELSE_TKN);Stmt();
            break;
        default:
            break;
    }
}

void
Stmt() {
    switch(lookAhead) {
        case ID_TKN:
            match(ID_TKN);match(ASSIGN_TKN);Expr();match(SEMICOLON_TKN);
            break;
        case BREAK_TKN:
            match(BREAK_TKN);match(SEMICOLON_TKN);
            break;
        case WHILE_TKN:
            match(WHILE_TKN);match(LEFT_BRACKET);Bool();match(RIGHT_BRACKET);Stmt();
            break;
        case DO_TKN:
            match(DO_TKN);Stmt();match(WHILE_TKN);
            match(LEFT_BRACKET);Bool();match(RIGHT_BRACKET);match(SEMICOLON_TKN);
            break;
        case LEFT_BRACE:
            Block();
            break;
        case IF_TKN:
            match(IF_TKN);match(LEFT_BRACKET);Bool();match(RIGHT_BRACKET);Stmt();Elsepart();
            break;
        default:
            printf("Stmt error (lookAhead:%d %s)\n", lookAhead, yytext);status++;
    }
}

void
Stmts() {
    switch(lookAhead) {
        case ID_TKN:
        case IF_TKN:
        case BREAK_TKN:
        case WHILE_TKN:
        case DO_TKN:
        case LEFT_BRACE:
            Stmt();Stmts();
        default:
            break;
    }
}

void
Block() {
    match( LEFT_BRACE );
    Stmts();
    match( RIGHT_BRACE );
}

void
Program() {
    Block();
}

int
main() { 
    char filename[1000];

    printf("Source filename:");
    gets(filename);
    BeginCompileOneFile(filename);

    lookAhead = yylex();
    Program();

    EndCompileOneFile();
    if(status == 0) {
        printf("\n=== PARSER ACCEPTED INPUT FILE ===\n");
    } else {
        printf("\n=== PARSER REJECTED INPUT FILE ===\n");
    }

    getchar();
    return 0;
}
