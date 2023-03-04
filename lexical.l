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
{dec_const} {printf("[%d][dec_const]: %d\n", yylineno, dec_decode(yytext));}
{oct_const} {printf("[%d][oct_const]: %d\n", yylineno, oct_decode(yytext));}
{hex_const} {printf("[%d][hex_const]: %d\n", yylineno, hex_decode(yytext));}
{FLOAT} {printf("[%d][FLOAT]: %f\n", yylineno, atof(yytext));}
{SEMI} {printf("[%d][SEMI]: %s\n", yylineno, yytext);}
{COMMA} {printf("[%d][COMMA]: %s\n", yylineno, yytext);}
{ASSIGNOP} {printf("[%d][ASSIGNOP]: %s\n", yylineno, yytext);}
{PLUS} {printf("[%d][PLUS]: %s\n", yylineno, yytext);}
{MINUS} {printf("[%d][MINUS]: %s\n", yylineno, yytext);}
{STAR} {printf("[%d][STAR]: %s\n", yylineno, yytext);}
{DIV} {printf("[%d][DIV]: %s\n", yylineno, yytext);}
{AND} {printf("[%d][AND]: %s\n", yylineno, yytext);}
{OR} {printf("[%d][OR]: %s\n", yylineno, yytext);}
{DOT} {printf("[%d][DOT]: %s\n", yylineno, yytext);}
{NOT} {printf("[%d][NOT]: %s\n", yylineno, yytext);}
{LP} {printf("[%d][LP]: %s\n", yylineno, yytext);}
{RP} {printf("[%d][RP]: %s\n", yylineno, yytext);}
{LB} {printf("[%d][LB]: %s\n", yylineno, yytext);}
{RB} {printf("[%d][RB]: %s\n", yylineno, yytext);}
{LC} {printf("[%d][LC]: %s\n", yylineno, yytext);}
{RC} {printf("[%d][RC]: %s\n", yylineno, yytext);}
{STRUCT} {printf("[%d][STRUCT]: %s\n", yylineno, yytext);}
{RETURN} {printf("[%d][RETURN]: %s\n", yylineno, yytext);}
{IF} {printf("[%d][IF]: %s\n", yylineno, yytext);}
{ELSE} {printf("[%d][ELSE]: %s\n", yylineno, yytext);}
{WHILE} {printf("[%d][WHILE]: %s\n", yylineno, yytext);}
{TYPE} {printf("[%d][TYPE]: %s\n", yylineno, yytext);}
{RELOP} {printf("[%d][RELOP]: %s\n", yylineno, yytext);}
{ID} {printf("[%d][ID]: %s\n", yylineno,yytext);}
\n { }
%%

