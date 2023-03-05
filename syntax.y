%{
#include "lex.yy.c"
#include "include/AST.h"
%}
%define api.pure
%union {
    int node_idx;
}
%token <node_idx> T_INT
%token <node_idx> T_FLOAT
%token <node_idx> T_SEMI
%token <node_idx> T_COMMA
%token <node_idx> T_ASSIGNOP
%token <node_idx> T_PLUS
%token <node_idx> T_MINUS
%token <node_idx> T_STAR
%token <node_idx> T_DIV
%token <node_idx> T_AND
%token <node_idx> T_OR
%token <node_idx> T_DOT
%token <node_idx> T_NOT
%token <node_idx> T_LP
%token <node_idx> T_RP
%token <node_idx> T_LB
%token <node_idx> T_RB
%token <node_idx> T_LC
%token <node_idx> T_RC
%token <node_idx> T_STRUCT
%token <node_idx> T_RETURN
%token <node_idx> T_IF  
%token <node_idx> T_ELSE  
%token <node_idx> T_WHILE  
%token <node_idx> T_TYPE  
%token <node_idx> T_RELOP  
%token <node_idx> T_ID

%type <node_idx> Program ExtDefList ExtDef ExtDecList
%type <node_idx> Specifier StructSpecifier OptTag Tag
%type <node_idx> VarDec FunDec VarList ParamDec
%type <node_idx> DecList Def DefList Dec
%type <node_idx> CompSt StmtList Stmt
%type <node_idx> Exp Args

%%
/* High-level Definitions {{{*/
Program: ExtDefList                             {$$ = new_node(Program, 1, $1); set_root($$);}
       ;
ExtDefList: /* empty */                         {value_t e = {.cnt_int=0,}; $$ = new_node(ExtDefList, 1, new_leaf(empty, e));}
          | ExtDef ExtDefList                   {$$ = new_node(ExtDefList, 2, $1, $2);}
          ;
ExtDef: Specifier ExtDecList T_SEMI             {$$ = new_node(ExtDef, 3, $1, $2, $3);}
      | Specifier T_SEMI                        {$$ = new_node(ExtDef, 2, $1, $2);}
      | Specifier FunDec CompSt                 {$$ = new_node(ExtDef, 3, $1, $2, $3);}
      ;
ExtDecList: VarDec                              {$$ = new_node(ExtDecList, 1, $1);}
          | VarDec T_COMMA ExtDecList           {$$ = new_node(ExtDecList, 3, $1, $2, $3);}
          ;
/*}}}*/
/* Specifiers {{{*/
Specifier: T_TYPE                               {$$ = new_node(Specifier, 1, $1);}
         | StructSpecifier                      {$$ = new_node(Specifier, 1, $1);}
         ;
StructSpecifier: T_STRUCT OptTag T_LC DefList T_RC    {$$ = new_node(StructSpecifier, 5, $1, $2, $3, $4, $5);}
               | T_STRUCT Tag                   {$$ = new_node(StructSpecifier, 2, $1, $2);}
               ;
OptTag: /* empty */                             {value_t e = {.cnt_int=0,}; $$ = new_node(ExtDefList, 1, new_leaf(empty, e));}
      | T_ID                                    {$$ = new_node(OptTag, 1, $1);}
      ;
Tag: T_ID                                       {$$ = new_node(Tag, 1, $1);}
   ;
/* }}} */
/* Declarators {{{*/ 
VarDec: T_ID                                    {$$ = new_node(VarDec, 1, $1);}
      | VarDec T_LB T_INT T_RB                  {$$ = new_node(VarDec, 4, $1, $2, $3, $4);}
      ;
FunDec: T_ID T_LP VarList T_RP                  {$$ = new_node(FunDec, 4, $1, $2, $3, $4);}
      | T_ID T_LP T_RP                          {$$ = new_node(FunDec, 3, $1, $2, $3);}
      ;
VarList: ParamDec T_COMMA VarList               {$$ = new_node(VarList, 3, $1, $2, $3);}
       | ParamDec                               {$$ = new_node(VarList, 1, $1);}
       ;
ParamDec: Specifier VarDec                      {$$ = new_node(ParamDec, 1, $1);}
        ;
/* }}} */
/* Statements {{{*/
CompSt: T_LC DefList StmtList T_RC              {$$ = new_node(CompSt, 4, $1, $2, $3, $4);}
      ;
StmtList: /* empty */                           {value_t e = {.cnt_int=0,}; $$ = new_node(ExtDefList, 1, new_leaf(empty, e));}
        |Stmt StmtList                          {$$ = new_node(StmtList, 2, $1, $2);}
        ;
Stmt: Exp T_SEMI                                {$$ = new_node(Stmt, 2, $1, $2);}
    | CompSt                                    {$$ = new_node(Stmt, 1, $1);}
    | T_RETURN Exp T_SEMI                       {$$ = new_node(Stmt, 3, $1, $2, $3);}
    | T_IF T_LP Exp T_RP Stmt                   {$$ = new_node(Stmt, 4, $1, $2, $3, $4);}
    | T_IF T_LP Exp T_RP Stmt T_ELSE Stmt       {$$ = new_node(Stmt, 7, $1, $2, $3, $4, $5, $6, $7);}
    | T_WHILE T_LP Exp T_RP Stmt                {$$ = new_node(Stmt, 5, $1, $2, $3, $4, $5);}
    ;
/* }}} */
/* Local Definitions {{{*/
DefList: /* empty */                            {value_t e = {.cnt_int=0,}; $$ = new_node(ExtDefList, 1, new_leaf(empty, e));}
       | Def DefList                            {$$ = new_node(DefList, 2, $1, $2);}
       ;
Def: Specifier DecList T_SEMI                   {$$ = new_node(Def, 3, $1, $2, $3);}
   ;
DecList: Dec                                    {$$ = new_node(DecList, 1, $1);}
       | Dec T_COMMA DecList                    {$$ = new_node(DecList, 3, $1, $2, $3);}
       ;
Dec: VarDec                                     {$$ = new_node(Dec, 1, $1);}
   | VarDec T_ASSIGNOP Exp                      {$$ = new_node(Dec, 3, $1, $2, $3);}
   ;
/*}}}*/
/* Expressions {{{*/
Exp: Exp T_ASSIGNOP Exp                         {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_AND Exp                              {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_OR Exp                               {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_RELOP Exp                            {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_PLUS Exp                             {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_MINUS Exp                            {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_STAR Exp                             {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_DIV Exp                              {$$ = new_node(Exp, 3, $1, $2, $3);}
   | T_LP Exp T_RP                              {$$ = new_node(Exp, 3, $1, $2, $3);}
   | T_ID T_LP T_RP                             {$$ = new_node(Exp, 3, $1, $2, $3);}
   | Exp T_DOT T_ID                             {$$ = new_node(Exp, 3, $1, $2, $3);}
   | T_MINUS Exp                                {$$ = new_node(Exp, 2, $1, $2);}
   | T_NOT Exp                                  {$$ = new_node(Exp, 2, $1, $2);}
   | T_ID T_LP Args T_RP                        {$$ = new_node(Exp, 4, $1, $2, $3, $4);}
   | Exp T_LB Exp T_RB                          {$$ = new_node(Exp, 4, $1, $2, $3, $4);}
   | T_ID                                       {$$ = new_node(Exp, 1, $1);}
   | T_INT                                      {$$ = new_node(Exp, 1, $1);}
   | T_FLOAT                                    {$$ = new_node(Exp, 1, $1);}
   ;
Args: Exp T_COMMA Args                          {$$ = new_node(Args, 3, $1, $2, $3);}
    | Exp                                       {$$ = new_node(Args, 1, $1);}
    ;
/* }}} */
%%
