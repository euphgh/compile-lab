%{
#include "syntax.tab.h"
#include "include/AST.h"
#include "include/debug.h"
#include <stdbool.h>
extern bool has_lexical_error;
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
static void remove_blk_cmt();
static int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line = yylineno; \
    yylloc->first_column = yycolumn; \
    yylloc->last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}
%option yylineno
%option bison-bridge bison-locations
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
{dec_const} {value_t t = { .cnt_int = dec_v(yytext), }; yylval->node_idx = new_leaf(INT,t);       Log("[%d][INT_dec]: %d", yylineno, dec_v(yytext)); return T_INT;}
{oct_const} {value_t t = { .cnt_int = oct_v(yytext), }; yylval->node_idx = new_leaf(INT,t);       Log("[%d][INT_oct]: %d", yylineno, oct_v(yytext)); return T_INT;}
{hex_const} {value_t t = { .cnt_int = hex_v(yytext), }; yylval->node_idx = new_leaf(INT,t);       Log("[%d][INT_hex]: %d", yylineno, hex_v(yytext)); return T_INT;}
{FLOAT}     {value_t t = { .cnt_flt = atof(yytext), };  yylval->node_idx = new_leaf(FLOAT,t);     Log("[%d][FLOAT]: %f", yylineno, atof(yytext)); return T_FLOAT;}
{SEMI}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(SEMI,t);      Log("[%d][SEMI]: %s", yylineno, yytext); return T_SEMI;}
{COMMA}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(COMMA,t);     Log("[%d][COMMA]: %s", yylineno, yytext); return T_COMMA;}
{ASSIGNOP}  {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(ASSIGNOP,t);  Log("[%d][ASSIGNOP]: %s", yylineno, yytext); return T_ASSIGNOP;}
{PLUS}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(PLUS,t);      Log("[%d][PLUS]: %s", yylineno, yytext); return T_PLUS;}
{MINUS}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(MINUS,t);     Log("[%d][MINUS]: %s", yylineno, yytext); return T_MINUS;}
{STAR}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(STAR,t);      Log("[%d][STAR]: %s", yylineno, yytext); return T_STAR;}
{DIV}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(DIV,t);       Log("[%d][DIV]: %s", yylineno, yytext); return T_DIV;}
{AND}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(AND,t);       Log("[%d][AND]: %s", yylineno, yytext); return T_AND;}
{OR}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(OR,t);        Log("[%d][OR]: %s", yylineno, yytext); return T_OR;}
{DOT}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(DOT,t);       Log("[%d][DOT]: %s", yylineno, yytext); return T_DOT;}
{NOT}       {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(NOT,t);       Log("[%d][NOT]: %s", yylineno, yytext); return T_NOT;}
{LP}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(LP,t);        Log("[%d][LP]: %s", yylineno, yytext); return T_LP;}
{RP}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(RP,t);        Log("[%d][RP]: %s", yylineno, yytext); return T_RP;}
{LB}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(LB,t);        Log("[%d][LB]: %s", yylineno, yytext); return T_LB;}
{RB}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(RB,t);        Log("[%d][RB]: %s", yylineno, yytext); return T_RB;}
{LC}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(LC,t);        Log("[%d][LC]: %s", yylineno, yytext); return T_LC;}
{RC}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(RC,t);        Log("[%d][RC]: %s", yylineno, yytext); return T_RC;}
{STRUCT}    {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(STRUCT,t);    Log("[%d][STRUCT]: %s", yylineno, yytext); return T_STRUCT;}
{RETURN}    {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(RETURN,t);    Log("[%d][RETURN]: %s", yylineno, yytext); return T_RETURN;}
{IF}        {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(IF,t);        Log("[%d][IF]: %s", yylineno, yytext); return T_IF;}
{ELSE}      {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(ELSE,t);      Log("[%d][ELSE]: %s", yylineno, yytext); return T_ELSE;}
{WHILE}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(WHILE,t);     Log("[%d][WHILE]: %s", yylineno, yytext); return T_WHILE;}
{TYPE}      {value_t t = { .id_lit = strdup(yytext), }; yylval->node_idx = new_leaf(TYPE,t);      Log("[%d][TYPE]: %s", yylineno, yytext); return T_TYPE;}
{RELOP}     {value_t t = { .cnt_int = 0, };             yylval->node_idx = new_leaf(RELOP,t);     Log("[%d][RELOP]: %s", yylineno, yytext); return T_RELOP;}
{ID}        {value_t t = { .id_lit = strdup(yytext), }; yylval->node_idx = new_leaf(ID,t);        Log("[%d][ID]: %s", yylineno,yytext);  return T_ID;}
"//" {Log ("[%d][line  cmt]: %s", yylineno,yytext); char c = input(); while (c != '\n') c = input(); }
"/*" {Log ("[%d][block cmt]: %s", yylineno,yytext); remove_blk_cmt();}
\n  { yycolumn = 1;}
\t+ { }
[[:space:]]+ {}
.           {fprintf(stdout, "Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext); has_lexical_error = true;}
%%
static void remove_blk_cmt(){/*{{{*/
    char first='\0', second = '\0';
    bool is_blk_end = false;
    bool is_eof = false;
    do {
        first = second;
        second = input();
        is_eof = second==0;
        is_blk_end = (first=='*') && (second=='/');
    } while(!is_blk_end && !is_eof);
}/*}}}*/
