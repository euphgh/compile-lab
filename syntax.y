%{
#include "lex.yy.c"
#include "ast.h"
#include "macro.h"
#define C_FMT
extern bool has_syntax_error;
void yyerror (YYLTYPE *locp, char const *msg){
    fprintf (stdout,  "Error type B at Line %d: %s\n", locp->first_line, msg);
    has_syntax_error = true;
}
%}
%locations 
%define api.pure full
%define parse.error custom
%define parse.lac full
%union {
    int node_idx;
}
%token <node_idx> T_INT T_FLOAT
%token <node_idx> T_SEMI T_COMMA

%right <node_idx> T_ASSIGNOP
%left  <node_idx> T_OR
%left  <node_idx> T_AND
%nonassoc <node_idx> T_RELOP  
%left  <node_idx> T_PLUS T_MINUS
%left  <node_idx> T_STAR T_DIV
%right <node_idx> T_NOT
%left <node_idx> T_DOT T_LB T_RB T_LP T_RP

%token <node_idx> T_LC
%token <node_idx> T_RC
%token <node_idx> T_STRUCT
%token <node_idx> T_RETURN
%token <node_idx> T_IF  
%nonassoc <node_idx> LOWER_THAN_ELSE 
%nonassoc <node_idx> T_ELSE
%token <node_idx> T_WHILE  
%token <node_idx> T_TYPE  
%token <node_idx> T_ID

/*{{{*/
%type <node_idx> Program ExtDefList ExtDef ExtDecList
%type <node_idx> Specifier StructSpecifier OptTag Tag
%type <node_idx> VarDec FunDec VarList ParamDec
%type <node_idx> DecList Def DefList Dec
%type <node_idx> CompSt StmtList Stmt
%type <node_idx> Exp Args
/*}}}*/
%%
/* High-level Definitions {{{*/
Program: ExtDefList                             {$$ = new_node(Program, 1, $1); set_root($$);}
       ;
ExtDefList: /* empty */                         {$$ = -1;}
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
OptTag: /* empty */                             {$$ = -1;}
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
ParamDec: Specifier VarDec                      {$$ = new_node(ParamDec,2, $1, $2);}
        ;
/* }}} */
/* Statements {{{*/
CompSt: T_LC DefList StmtList T_RC              {$$ = new_node(CompSt, 4, $1, $2, $3, $4);}
      ;
StmtList: /* empty */                           {$$ = -1;}
        |Stmt StmtList                          {$$ = new_node(StmtList, 2, $1, $2);}
        ;
Stmt: Exp T_SEMI                                {$$ = new_node(Stmt, 2, $1, $2);}
    | CompSt                                    {$$ = new_node(Stmt, 1, $1);}
    | T_RETURN Exp T_SEMI                       {$$ = new_node(Stmt, 3, $1, $2, $3);}
    | T_IF T_LP Exp T_RP Stmt %prec LOWER_THAN_ELSE     {$$ = new_node(Stmt, 5, $1, $2, $3, $4, $5);}
    | T_IF T_LP Exp T_RP Stmt T_ELSE Stmt       {$$ = new_node(Stmt, 7, $1, $2, $3, $4, $5, $6, $7);}
    | T_WHILE T_LP Exp T_RP Stmt                {$$ = new_node(Stmt, 5, $1, $2, $3, $4, $5);}
    | error T_SEMI                              {$$ = new_node(Stmt, 1, $2);}                    
    ;
/* }}} */
/* Local Definitions {{{*/
DefList: /* empty */                            {$$ = -1;}
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
static const char* yysymbol_to_str(yysymbol_kind_t sym){/*{{{*/
    const char* res = NULL;
    switch (sym){
#define yysymbol_name_map(x, is_ter, str) \
        case MUXONE(is_ter, YYSYMBOL_T_##x, YYSYMBOL_##x):\
            res = str; \
            break;
        AllSyntaxSymbol(yysymbol_name_map)
        default:
            res = yysymbol_name(sym);
            break;
    }
    return res;
}/*}}}*/
static int yyreport_syntax_error (const yypcontext_t *ctx) {/*{{{*/
  int res = 0;
  char emsg_buf[128];
  char* p = emsg_buf;
  // Report the tokens expected at this point.
  {
    enum { TOKENMAX = 5 };
    yysymbol_kind_t expected[TOKENMAX];
    int n = yypcontext_expected_tokens (ctx, expected, TOKENMAX);
    // Forward errors to yyparse.
    if (n < 0)  res = n;
    else
      for (int i = 0; i < n; ++i)
        p += sprintf (p, "%s \"%s\".",
                 i == 0 ? "expected" : " or", yysymbol_to_str (expected[i]));
  }
  // Report the unexpected token.
  {
    yysymbol_kind_t lookahead = yypcontext_token (ctx);
    if (lookahead != YYSYMBOL_YYEMPTY)
        p += sprintf (p, "unexpected \"%s\".", yysymbol_to_str (lookahead));
  }
  yyerror(yypcontext_location (ctx), emsg_buf);
  return res;
}/*}}}*/
