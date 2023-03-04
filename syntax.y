%{
#include "lex.yy.c"
%}
%pure_parser
%union {
    int cst_int;
    float cst_flt;
    char* literals;
}
%token <cst_int>    INT
%token <cst_flt>    FLOAT
%token SEMI  
%token COMMA  
%token ASSIGNOP  
%token PLUS  
%token MINUS  
%token STAR  
%token DIV  
%token AND  
%token OR  
%token DOT  
%token NOT  
%token LP  
%token RP  
%token LB  
%token RB  
%token LC  
%token RC  
%token STRUCT  
%token RETURN  
%token IF  
%token ELSE  
%token WHILE  
%token <literals>   TYPE  
%token <literals>   RELOP  
%token <literals>   ID
%%
/* High-level Definitions {{{*/
Program: /* empty */ 
       ExtDefList
       ;
ExtDefList: /* empty */ 
          | ExtDef ExtDefList
          ;
ExtDef: Specifier ExtDecList SEMI
      | Specifier SEMI
      | Specifier FunDec CompSt
      ;
ExtDecList: VarDec
          | VarDec COMMA ExtDecList
          ;
/*}}}*/
/* Specifiers {{{*/
Specifier: TYPE 
         | StructSpecifier
         ;
StructSpecifier: STRUCT OptTag LC DefList RC
               | STRUCT Tag
OptTag: /* empty */ 
      | ID
      ;
Tag: ID 
   ;
/* }}} */
/* Declarators {{{*/ 
VarDec: ID
      | VarDec LB INT RB
      ;
FunDec: ID LP VarList RP
      | ID LP RP
      ;
VarList: ParamDec COMMA VarList
       | ParamDec
       ;
ParamDec: Specifier VarDec 
        ;
/* }}} */
/* Statements {{{*/
CompSt: LC DefList StmtList RC 
      ;
StmtList: /* empty */  
        |Stmt StmtList
        ;
Stmt: Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;
/* }}} */
/* Local Definitions {{{*/
DefList: /* empty */  
       | Def DefList
       ;
Def: Specifier DecList SEMI 
   ;
DecList: Dec
       | Dec COMMA DecList
       ;
Dec: VarDec
   | VarDec ASSIGNOP Exp
   ;
/*}}}*/
/* Expressions {{{*/
Exp: Exp ASSIGNOP Exp
   | Exp AND Exp
   | Exp OR Exp
   | Exp RELOP Exp
   | Exp PLUS Exp
   | Exp MINUS Exp
   | Exp STAR Exp
   | Exp DIV Exp
   | LP Exp RP
   | MINUS Exp
   | NOT Exp
   | ID LP Args RP
   | ID LP RP
   | Exp LB Exp RB
   | Exp DOT ID
   | ID
   | INT
   | FLOAT
   ;
Args: Exp COMMA Args
    | Exp
    ;
/* }}} */
%%
