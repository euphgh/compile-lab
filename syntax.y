%{
#include "lex.yy.c"
#include "include/AST.h"
%}
%define api.pure
%union {
    int node_idx;
}
%token <node_idx> INT
%token <node_idx> FLOAT
%token <node_idx> SEMI
%token <node_idx> COMMA
%token <node_idx> ASSIGNOP
%token <node_idx> PLUS
%token <node_idx> MINUS
%token <node_idx> STAR
%token <node_idx> DIV
%token <node_idx> AND
%token <node_idx> OR
%token <node_idx> DOT
%token <node_idx> NOT
%token <node_idx> LP
%token <node_idx> RP
%token <node_idx> LB
%token <node_idx> RB
%token <node_idx> LC
%token <node_idx> RC
%token <node_idx> STRUCT
%token <node_idx> RETURN
%token <node_idx> IF  
%token <node_idx> ELSE  
%token <node_idx> WHILE  
%token <node_idx> TYPE  
%token <node_idx> RELOP  
%token <node_idx> ID

%type <node_idx> Program ExtDefList ExtDef ExtDecList
%type <node_idx> Specifier StructSpecifier OptTag Tag
%type <node_idx> VarDec FunDec VarList ParamDec
%type <node_idx> DecList Def DefList Dec
%type <node_idx> CompSt StmtList Stmt
%type <node_idx> Exp Args

%%
/* High-level Definitions {{{*/
Program: ExtDefList                             {new_node(Program, 1, $1);}
       ;
ExtDefList: /* empty */                         {value_t e = {.cnt_int=0,}; new_node(ExtDefList, 1, new_leaf(empty, e));}
          | ExtDef ExtDefList                   {new_node(ExtDefList, 2, $1, $2);}
          ;
ExtDef: Specifier ExtDecList SEMI               {new_node(ExtDef, 3, $1, $2, $3);}
      | Specifier SEMI                          {new_node(ExtDef, 2, $1, $2);}
      | Specifier FunDec CompSt                 {new_node(ExtDef, 3, $1, $2, $3);}
      ;
ExtDecList: VarDec                              {new_node(ExtDecList, 1, $1);}
          | VarDec COMMA ExtDecList             {new_node(ExtDecList, 3, $1, $2, $3);}
          ;
/*}}}*/
/* Specifiers {{{*/
Specifier: TYPE                                 {new_node(Specifier, 1, $1);}
         | StructSpecifier                      {new_node(Specifier, 1, $1);}
         ;
StructSpecifier: STRUCT OptTag LC DefList RC    {new_node(StructSpecifier, 5, $1, $2, $3, $4, $5);}
               | STRUCT Tag                     {new_node(StructSpecifier, 2, $1, $2);}
               ;
OptTag: /* empty */                             {value_t e = {.cnt_int=0,}; new_node(ExtDefList, 1, new_leaf(empty, e));}
      | ID                                      {new_node(OptTag, 1, $1);}
      ;
Tag: ID                                         {new_node(Tag, 1, $1);}
   ;
/* }}} */
/* Declarators {{{*/ 
VarDec: ID                                      {new_node(VarDec, 1, $1);}
      | VarDec LB INT RB                        {new_node(VarDec, 4, $1, $2, $3, $4);}
      ;
FunDec: ID LP VarList RP                        {new_node(FunDec, 4, $1, $2, $3, $4);}
      | ID LP RP                                {new_node(FunDec, 3, $1, $2, $3);}
      ;
VarList: ParamDec COMMA VarList                 {new_node(VarList, 3, $1, $2, $3);}
       | ParamDec                               {new_node(VarList, 1, $1);}
       ;
ParamDec: Specifier VarDec                      {new_node(ParamDec, 1, $1);}
        ;
/* }}} */
/* Statements {{{*/
CompSt: LC DefList StmtList RC                  {new_node(CompSt, 4, $1, $2, $3, $4);}
      ;
StmtList: /* empty */                           {value_t e = {.cnt_int=0,}; new_node(ExtDefList, 1, new_leaf(empty, e));}
        |Stmt StmtList                          {new_node(StmtList, 2, $1, $2);}
        ;
Stmt: Exp SEMI                                  {new_node(Stmt, 2, $1, $2);}
    | CompSt                                    {new_node(Stmt, 1, $1);}
    | RETURN Exp SEMI                           {new_node(Stmt, 3, $1, $2, $3);}
    | IF LP Exp RP Stmt                         {new_node(Stmt, 4, $1, $2, $3, $4);}
    | IF LP Exp RP Stmt ELSE Stmt               {new_node(Stmt, 7, $1, $2, $3, $4, $5, $6, $7);}
    | WHILE LP Exp RP Stmt                      {new_node(Stmt, 5, $1, $2, $3, $4, $5);}
    ;
/* }}} */
/* Local Definitions {{{*/
DefList: /* empty */                            {value_t e = {.cnt_int=0,}; new_node(ExtDefList, 1, new_leaf(empty, e));}
       | Def DefList                            {new_node(DefList, 2, $1, $2);}
       ;
Def: Specifier DecList SEMI                     {new_node(Def, 3, $1, $2, $3);}
   ;
DecList: Dec                                    {new_node(DecList, 1, $1);}
       | Dec COMMA DecList                      {new_node(DecList, 3, $1, $2, $3);}
       ;
Dec: VarDec                                     {new_node(Dec, 1, $1);}
   | VarDec ASSIGNOP Exp                        {new_node(Dec, 3, $1, $2, $3);}
   ;
/*}}}*/
/* Expressions {{{*/
Exp: Exp ASSIGNOP Exp                           {new_node(Exp, 3, $1, $2, $3);}
   | Exp AND Exp                                {new_node(Exp, 3, $1, $2, $3);}
   | Exp OR Exp                                 {new_node(Exp, 3, $1, $2, $3);}
   | Exp RELOP Exp                              {new_node(Exp, 3, $1, $2, $3);}
   | Exp PLUS Exp                               {new_node(Exp, 3, $1, $2, $3);}
   | Exp MINUS Exp                              {new_node(Exp, 3, $1, $2, $3);}
   | Exp STAR Exp                               {new_node(Exp, 3, $1, $2, $3);}
   | Exp DIV Exp                                {new_node(Exp, 3, $1, $2, $3);}
   | LP Exp RP                                  {new_node(Exp, 3, $1, $2, $3);}
   | ID LP RP                                   {new_node(Exp, 3, $1, $2, $3);}
   | Exp DOT ID                                 {new_node(Exp, 3, $1, $2, $3);}
   | MINUS Exp                                  {new_node(Exp, 2, $1, $2);}
   | NOT Exp                                    {new_node(Exp, 2, $1, $2);}
   | ID LP Args RP                              {new_node(Exp, 4, $1, $2, $3, $4);}
   | Exp LB Exp RB                              {new_node(Exp, 4, $1, $2, $3, $4);}
   | ID                                         {new_node(Exp, 1, $1);}
   | INT                                        {new_node(Exp, 1, $1);}
   | FLOAT                                      {new_node(Exp, 1, $1);}
   ;
Args: Exp COMMA Args                            {new_node(Args, 3, $1, $2, $3);}
    | Exp                                       {new_node(Args, 1, $1);}
    ;
/* }}} */
%%
