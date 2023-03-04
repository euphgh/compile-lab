#ifndef __AST_H__
#define __AST_H__

#define AllSyntaxSymbol(_) \
    _(Program) _(ExtDefList) _(ExtDef) _(ExtDecList) \
    _(Specifier) _(StructSpecifier) _(OptTag) _(Tag) \
    _(VarDec) _(FunDec) _(VarList) _(ParamDec) \
    _(CompSt) _(StmtList) _(Stmt) \
    _(DefList) _(Def) _(DecList) _(Dec) \
    _(Exp) _(Args) \
    _(T_INT) _(T_FLOAT) \
    _(T_SEMI) _(T_COMMA) \
    _(T_ASSIGNOP) \
    _(T_PLUS) _(T_MINUS) _(T_STAR) _(T_DIV) \
    _(T_AND)  _(T_OR) \
    _(T_DOT) \
    _(T_NOT) \
    _(T_LP) _(T_RP) \
    _(T_LB) _(T_RB) \
    _(T_LC) _(T_RC) \
    _(T_STRUCT) \
    _(T_RETURN) \
    _(T_IF) _(T_ELSE) \
    _(T_WHILE) \
    _(T_TYPE) \
    _(T_RELOP) \
    _(T_ID) \
    _(empty)

typedef enum{
#define synt_t_enum(x) x,
    AllSyntaxSymbol(synt_t_enum)
} synt_t;
typedef union {
    int cnt_int;
    float cnt_flt;
    const char* id_lit;
} value_t;
typedef struct {
    int par_idx;
    int cld_idx[8];
    int cld_nr;
    synt_t synt_sym;
    value_t attrib; 
} node_t;

int new_leaf(synt_t synt_sym, value_t attrib);
int new_node(synt_t synt_sym, int cld_nr, ...);
void set_root(int node_idx);

#endif /* __AST_H__ */
