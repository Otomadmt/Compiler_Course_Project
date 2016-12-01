
%{

/* Variables and Numbers  */
#define ID_TKN      500
#define NUM_TKN     505

/* Reserved */
#define BASIC_TKN       510
#define SEMICOLON_TKN   511
#define COMMA_TKN       512

/* Operators */
#define OP_ADMI_TKN 515
#define OP_MUDI_TKN 516
#define ASSIGN_TKN  517

/* Relational Operators */
#define RELOP_TKN   520
#define RELOP_LT    521
#define RELOP_LE    522
#define RELOP_EQ    523
#define RELOP_NE    524
#define RELOP_GT    525
#define RELOP_GE    526

/* Brackets */
#define LEFT_BRACKET            530
#define RIGHT_BRACKET           531
#define LEFT_SQUARE_BRACKET     532
#define RIGHT_SQUARE_BRACKET    533
#define LEFT_BRACE              534
#define RIGHT_BRACE             535

/* Conditional Statements */
#define IF_TKN      610
#define ELSE_TKN    611
#define WHILE_TKN   612
#define DO_TKN      613
#define BREAK_TKN   614

/* Boolean Statements */
#define TRUE_TKN    620
#define FALSE_TKN   621


int yylval;
int installID() ;
int installNum() ;
%}

delim   [" "\n\t\r]
ws      {delim}+
letter  [A-Za-z]
digit   [0-9]
basic   int|char|float|bool|real
opadmi  ["+"-]
opmudi  ["*"/]
assign  [=]
id      (_|{letter})(_|{letter}|{digit})*
number  {digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%

{ws}        {}

{basic}     { printf("BASIC:%s\n", yytext); return (BASIC_TKN); }
if          { printf("IF:%s\n", yytext);   return (IF_TKN); }
else        { printf("ELSE:%s\n", yytext); return (ELSE_TKN); }
while       { printf("WHILE:%s\n", yytext); return (WHILE_TKN); }
do          { printf("DO:%s\n", yytext); return (DO_TKN); }
break       { printf("BREAK:%s\n", yytext); return (BREAK_TKN); }

true        { printf("TRUE:%s\n", yytext); return (TRUE_TKN); }
false       { printf("FALSE:%s\n", yytext); return (FALSE_TKN); }

{id}        { yylval = (int) installID(); printf("ID:%s\n", yytext); return (ID_TKN); }
{number}    { yylval = (int) installNum();printf("Num:%s\n", yytext); return (NUM_TKN); } 
{opadmi}    { yylval = OP_ADMI_TKN; printf("OP:%s\n", yytext); return(OP_ADMI_TKN); }
{opmudi}    { yylval = OP_MUDI_TKN; printf("OP:%s\n", yytext); return(OP_MUDI_TKN); }
{assign}    { yylval = ASSIGN_TKN; printf("OP:%s\n", yytext); return(ASSIGN_TKN); }
","         { yylval = COMMA_TKN; printf("COMMA:%s\n", yytext); return(COMMA_TKN); }
";"         { yylval = SEMICOLON_TKN; printf("SEMICOLON:%s\n", yytext); return(SEMICOLON_TKN); }
"<"         { yylval = RELOP_LT; printf("LT:%s\n", yytext);  return(RELOP_TKN); }
"<="        { yylval = RELOP_LE; printf("LE:%s\n", yytext);  return(RELOP_TKN); }
"=="        { yylval = RELOP_EQ; printf("EQ:%s\n", yytext);  return(RELOP_TKN); }
"<>"        { yylval = RELOP_NE; printf("NE:%s\n", yytext);  return(RELOP_TKN); }
"!="        { yylval = RELOP_NE; printf("NE:%s\n", yytext);  return(RELOP_TKN); }
">"         { yylval = RELOP_GT; printf("GT:%s\n", yytext);  return(RELOP_TKN); }
">="        { yylval = RELOP_GE; printf("GE:%s\n", yytext);  return(RELOP_TKN); }

\(          { yylval = LEFT_BRACKET; printf("LEFT_BRACKET:%s\n", yytext);  return (LEFT_BRACKET); }
\)          { yylval = RIGHT_BRACKET; printf("RIGHT_BRACKET:%s\n", yytext);  return (RIGHT_BRACKET); }
\[          { yylval = LEFT_SQUARE_BRACKET; printf("LEFT_SQUARE_BRACKET:%s\n", yytext);  return (LEFT_SQUARE_BRACKET); }
\]          { yylval = RIGHT_SQUARE_BRACKET; printf("RIGHT_SQUARE_BRACKET:%s\n", yytext);  return (RIGHT_SQUARE_BRACKET); }
\{          { yylval = LEFT_BRACE; printf("LEFT_BRACE:%s\n", yytext);  return (LEFT_BRACE); }
\}          { yylval = RIGHT_BRACE; printf("RIGHT_BRACE:%s\n", yytext);  return (RIGHT_BRACE); }

[\/][\*]([^\*])*[\*]([^\*\/](([^\*])*)[\*]|[\*])*(\/)   { printf("Remark:%s\n",yytext); }

%%

/* Set up yyin for yylex to read string from */
void
BeginCompileOneFile( const char * filename )
{  
   yyin = fopen( filename, "r");
   fseek( yyin, 0, SEEK_SET );
}

/* Close yyin after compile */
void
EndCompileOneFile(void) 
{
   fclose(yyin);
   yyin = 0;
}

int
yywrap() {
    return 1;
}

int
installID() {
   return 1;
}

int
installNum() {
   return 1;
}

// int
// main() { 
//     int token;
//     char filename[1000];

//     printf("Source file:");
//     gets(filename);
//     BeginCompileOneFile(filename);

//     // Scan to EOF
//     while( ( token = yylex() ) > 0 ) ;

//     EndCompileOneFile();

//     getchar();
//     return 0;
// }

