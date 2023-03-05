#ifndef __AST_H__
#define __AST_H__

/* syntax symbol list {{{*/
#define T_SET 1
#define V_SET 0
#define AllSyntaxSymbol(_) \
    _(Program, V_SET) \
    _(ExtDefList, V_SET) \
    _(ExtDef, V_SET) \
    _(ExtDecList, V_SET) \
    _(Specifier, V_SET) \
    _(StructSpecifier, V_SET) \
    _(OptTag, V_SET) \
    _(Tag, V_SET) \
    _(VarDec, V_SET) \
    _(FunDec, V_SET) \
    _(VarList, V_SET) \
    _(ParamDec, V_SET) \
    _(CompSt, V_SET) \
    _(StmtList, V_SET) \
    _(Stmt, V_SET) \
    _(DefList, V_SET) \
    _(Def, V_SET) \
    _(DecList, V_SET) \
    _(Dec, V_SET) \
    _(Exp, V_SET) \
    _(Args, V_SET) \
    _(INT, T_SET) \
    _(FLOAT, T_SET) \
    _(SEMI, T_SET) \
    _(COMMA, T_SET) \
    _(ASSIGNOP, T_SET) \
    _(PLUS, T_SET) \
    _(MINUS, T_SET) \
    _(STAR, T_SET) \
    _(DIV, T_SET) \
    _(AND, T_SET)  \
    _(OR, T_SET) \
    _(DOT, T_SET) \
    _(NOT, T_SET) \
    _(LP, T_SET) \
    _(RP, T_SET) \
    _(LB, T_SET) \
    _(RB, T_SET) \
    _(LC, T_SET) \
    _(RC, T_SET) \
    _(STRUCT, T_SET) \
    _(RETURN, T_SET) \
    _(IF, T_SET) \
    _(ELSE, T_SET) \
    _(WHILE, T_SET) \
    _(TYPE, T_SET) \
    _(RELOP, T_SET) \
    _(ID, T_SET) \
    _(empty, T_SET)
/*}}}*/

typedef enum{
#define synt_t_enum(x, is_ter_sym) x,
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
void print_from_root();

#endif /* __AST_H__ */
