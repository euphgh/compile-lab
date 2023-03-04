%{
#include "syntax.tab.h"
#include "include/AST.h"
static int dec_v(char* text) {/*{{{*/
    int dec = 0;
    sscanf(text, "%d", &dec);
    return dec;
}/*}}}*/
static int oct_v(char* text) {/*{{{*/
    int oct = 0;
    sscanf(text, "%o", &oct);
    return oct;
}/*}}}*/
static int hex_v(char* text) {/*{{{*/
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
{dec_const} {value_t t = { .cnt_int = dec_v(yytext), }; yylval->node_idx = new_leaf(T_INT,t);       printf("[%d][dec_INT]: %d\n", yylineno, dec_v(yytext)); return INT;}
{oct_const} {value_t t = { .cnt_int = oct_v(yytext), }; yylval->node_idx = new_leaf(T_INT,t);       printf("[%d][oct_INT]: %d\n", yylineno, oct_v(yytext)); return INT;}
{hex_const} {value_t t = { .cnt_int = hex_v(yytext), }; yylval->node_idx = new_leaf(T_INT,t);       printf("[%d][hex_INT]: %d\n", yylineno, hex_v(yytext)); return INT;}
{FLOAT}     {value_t t = { .cnt_flt = atof(yytext), };  yylval->node_idx = new_leaf(T_FLOAT,t);     printf("[%d][FLOAT]: %f\n", yylineno, atof(yytext)); return FLOAT;}
{SEMI}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_SEMI,t);      printf("[%d][SEMI]: %s\n", yylineno, yytext); return SEMI;}
{COMMA}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_COMMA,t);     printf("[%d][COMMA]: %s\n", yylineno, yytext); return COMMA;}
{ASSIGNOP}  {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_ASSIGNOP,t);  printf("[%d][ASSIGNOP]: %s\n", yylineno, yytext); return ASSIGNOP;}
{PLUS}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_PLUS,t);      printf("[%d][PLUS]: %s\n", yylineno, yytext); return PLUS;}
{MINUS}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_MINUS,t);     printf("[%d][MINUS]: %s\n", yylineno, yytext); return MINUS;}
{STAR}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_STAR,t);      printf("[%d][STAR]: %s\n", yylineno, yytext); return STAR;}
{DIV}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_DIV,t);       printf("[%d][DIV]: %s\n", yylineno, yytext); return DIV;}
{AND}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_AND,t);       printf("[%d][AND]: %s\n", yylineno, yytext); return AND;}
{OR}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_OR,t);        printf("[%d][OR]: %s\n", yylineno, yytext); return OR;}
{DOT}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_DOT,t);       printf("[%d][DOT]: %s\n", yylineno, yytext); return DOT;}
{NOT}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_NOT,t);       printf("[%d][NOT]: %s\n", yylineno, yytext); return NOT;}
{LP}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_LP,t);        printf("[%d][LP]: %s\n", yylineno, yytext); return LP;}
{RP}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_RP,t);        printf("[%d][RP]: %s\n", yylineno, yytext); return RP;}
{LB}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_LB,t);        printf("[%d][LB]: %s\n", yylineno, yytext); return LB;}
{RB}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_RB,t);        printf("[%d][RB]: %s\n", yylineno, yytext); return RB;}
{LC}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_LC,t);        printf("[%d][LC]: %s\n", yylineno, yytext); return LC;}
{RC}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_RC,t);        printf("[%d][RC]: %s\n", yylineno, yytext); return RC;}
{STRUCT}    {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_STRUCT,t);    printf("[%d][STRUCT]: %s\n", yylineno, yytext); return STRUCT;}
{RETURN}    {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_RETURN,t);    printf("[%d][RETURN]: %s\n", yylineno, yytext); return RETURN;}
{IF}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_IF,t);        printf("[%d][IF]: %s\n", yylineno, yytext); return IF;}
{ELSE}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_ELSE,t);      printf("[%d][ELSE]: %s\n", yylineno, yytext); return ELSE;}
{WHILE}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_WHILE,t);     printf("[%d][WHILE]: %s\n", yylineno, yytext); return WHILE;}
{TYPE}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_TYPE,t);      printf("[%d][TYPE]: %s\n", yylineno, yytext); return TYPE;}
{RELOP}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(T_RELOP,t);     printf("[%d][RELOP]: %s\n", yylineno, yytext); return RELOP;}
{ID}        {value_t t = { .id_lit = strdup(yytext), }; yylval->node_idx = new_leaf(T_ID,t);        printf("[%d][ID]: %s\n", yylineno,yytext);  return ID;}
\n { }
%%

