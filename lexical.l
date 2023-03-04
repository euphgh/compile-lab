%{
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
ID [[:alpha:]](([[:alnum:]]|_){0,31})
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
%%
{dec_const} {printf("[%d][dec_const]: %d\n", yylineno, dec_decode(yytext));}
{oct_const} {printf("[%d][oct_const]: %d\n", yylineno, oct_decode(yytext));}
{hex_const} {printf("[%d][hex_const]: %d\n", yylineno, hex_decode(yytext));}
{FLOAT} {printf("[%d][FLOAT]: %f\n", yylineno, atof(yytext));}
{ID} {printf("[%d][ID]: %s\n", yylineno,yytext);}
\n { }
%%

