%{
#include "lex.yy.c"
#include "include/debug.h"
#include "include/AST.h"
%}
%pure_parser
%union {
    int node_idx;
}
%token INT
%token FLOAT
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
%token TYPE  
%token RELOP  
%token ID
%%
int new_leaf(synt_t synt_sym, value_t attrib);
int new_node(synt_t synt_sym, int cld_nr, ...);
void set_root(int node_idx);
/* High-level Definitions {{{*/
Program: ExtDefList                             {new_node(Program, 1, $1)}
       ;
ExtDefList: /* empty */                         {new_node(ExtDefList, 1, new_leaf(empty, 0))}
          | ExtDef ExtDefList                   {new_node(ExtDefList, 2, $1, $2)}
          ;
ExtDef: Specifier ExtDecList SEMI               {new_node(ExtDef, 3, $1, $2, $3)}
      | Specifier SEMI                          {new_node(ExtDef, 2, $1, $2)}
      | Specifier FunDec CompSt                 {new_node(ExtDef, 3, $1, $2, $3)}
      ;
ExtDecList: VarDec                              {new_node(ExtDecList, 1, $1)}
          | VarDec COMMA ExtDecList             {new_node(ExtDecList, 3, $1, $2, $3)}
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
