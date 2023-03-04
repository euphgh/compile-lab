%{
#include "syntax.tab.h"
static int dec_decode(char* text) {/*{{{*/
    int dec = 0;
    sscanf(text, "%d", &dec);
    return dec;
}/*}}}*/
static int oct_decode(char* text) {/*{{{*/
    int oct = 0;
    sscanf(text, "%o", &oct);
    return oct;
}/*}}}*/
static int hex_decode(char* text) {/*{{{*/
    int hex = 0;
    sscanf(text, "%x", &hex);
    return hex;
}/*}}}*/
%}
%option yylineno
%option bison-bridge
digit [0-9]
digit_seq {digit}+
nonzore_digit [1-9]
hex_prefix 0(X|x)
oct_digit [0-7]
hex_digit [0-9A-Fa-f]
dec_const {nonzore_digit}{digit}*
oct_const 0{oct_digit}*
hex_const {hex_prefix}{hex_digit}+
fra_const ({digit_seq}".")|({digit_seq}?"."{digit_seq})
expt_part [eE][+-]?{digit_seq}

INT {dec_const}|{oct_const}|{hex_digit}
FLOAT {fra_const}{expt_part}?
SEMI ";"
COMMA ","
ASSIGNOP "="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT "."
NOT "!"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT "struct"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"
TYPE "int"|"float"
RELOP ">"|"<"|">="|"<="|"=="|"!="
ID [[:alpha:]](([[:alnum:]]|_){0,31})
%%
{dec_const} {yylval->cst_int = dec_decode(yytext);  printf("[%d][dec_const]: %d\n", yylineno, dec_decode(yytext)); return INT;}
{oct_const} {yylval->cst_int = oct_decode(yytext);  printf("[%d][oct_const]: %d\n", yylineno, oct_decode(yytext)); return INT;}
{hex_const} {yylval->cst_int = hex_decode(yytext);  printf("[%d][hex_const]: %d\n", yylineno, hex_decode(yytext)); return INT;}
{FLOAT}     {yylval->cst_flt = atof(yytext);        printf("[%d][FLOAT]: %f\n", yylineno, atof(yytext)); return FLOAT;}
{SEMI}      {printf("[%d][SEMI]: %s\n", yylineno, yytext); return SEMI;}
{COMMA}     {printf("[%d][COMMA]: %s\n", yylineno, yytext); return COMMA;}
{ASSIGNOP}  {printf("[%d][ASSIGNOP]: %s\n", yylineno, yytext); return ASSIGNOP;}
{PLUS}      {printf("[%d][PLUS]: %s\n", yylineno, yytext); return PLUS;}
{MINUS}     {printf("[%d][MINUS]: %s\n", yylineno, yytext); return MINUS;}
{STAR}      {printf("[%d][STAR]: %s\n", yylineno, yytext); return STAR;}
{DIV}       {printf("[%d][DIV]: %s\n", yylineno, yytext); return DIV;}
{AND}       {printf("[%d][AND]: %s\n", yylineno, yytext); return AND;}
{OR}        {printf("[%d][OR]: %s\n", yylineno, yytext); return OR;}
{DOT}       {printf("[%d][DOT]: %s\n", yylineno, yytext); return DOT;}
{NOT}       {printf("[%d][NOT]: %s\n", yylineno, yytext); return NOT;}
{LP}        {printf("[%d][LP]: %s\n", yylineno, yytext); return LP;}
{RP}        {printf("[%d][RP]: %s\n", yylineno, yytext); return RP;}
{LB}        {printf("[%d][LB]: %s\n", yylineno, yytext); return LB;}
{RB}        {printf("[%d][RB]: %s\n", yylineno, yytext); return RB;}
{LC}        {printf("[%d][LC]: %s\n", yylineno, yytext); return LC;}
{RC}        {printf("[%d][RC]: %s\n", yylineno, yytext); return RC;}
{STRUCT}    {printf("[%d][STRUCT]: %s\n", yylineno, yytext); return STRUCT;}
{RETURN}    {printf("[%d][RETURN]: %s\n", yylineno, yytext); return RETURN;}
{IF}        {printf("[%d][IF]: %s\n", yylineno, yytext); return IF;}
{ELSE}      {printf("[%d][ELSE]: %s\n", yylineno, yytext); return ELSE;}
{WHILE}     {printf("[%d][WHILE]: %s\n", yylineno, yytext); return WHILE;}
{TYPE}      {printf("[%d][TYPE]: %s\n", yylineno, yytext); return TYPE;}
{RELOP}     {printf("[%d][RELOP]: %s\n", yylineno, yytext); return RELOP;}
{ID}        {printf("[%d][ID]: %s\n", yylineno,yytext); yylval->literals = strdup(yytext); return ID;}
\n { }
%%

