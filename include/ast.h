#ifndef __AST_H__
#define __AST_H__

/* syntax symbol list {{{*/
#define T_SET 1
#define V_SET 0
#define AllSyntaxSymbol(_) \
    _(Program, V_SET, "Program") \
    _(ExtDefList, V_SET, "ExtDefList") \
    _(ExtDef, V_SET, "ExtDef") \
    _(ExtDecList, V_SET, "ExtDecList") \
    _(Specifier, V_SET, "Specifier") \
    _(StructSpecifier, V_SET, "StructSpecifier") \
    _(OptTag, V_SET, "OptTag") \
    _(Tag, V_SET, "Tag") \
    _(VarDec, V_SET, "VarDec") \
    _(FunDec, V_SET, "FunDec") \
    _(VarList, V_SET, "VarList") \
    _(ParamDec, V_SET, "ParamDec") \
    _(CompSt, V_SET, "CompSt") \
    _(StmtList, V_SET, "StmtList") \
    _(Stmt, V_SET, "Stmt") \
    _(DefList, V_SET, "DefList") \
    _(Def, V_SET, "Def") \
    _(DecList, V_SET, "DecList") \
    _(Dec, V_SET, "Dec") \
    _(Exp, V_SET, "Exp") \
    _(Args, V_SET, "Args") \
    _(INT, T_SET, "INT") \
    _(FLOAT, T_SET, "FLOAT") \
    _(SEMI, T_SET, ";") \
    _(COMMA, T_SET, ",") \
    _(ASSIGNOP, T_SET, "=") \
    _(PLUS, T_SET, "+") \
    _(MINUS, T_SET, "-") \
    _(STAR, T_SET, "*") \
    _(DIV, T_SET, "/") \
    _(AND, T_SET, "&&")  \
    _(OR, T_SET, "||") \
    _(DOT, T_SET, ".") \
    _(NOT, T_SET, "!") \
    _(LP, T_SET, "(") \
    _(RP, T_SET, ")") \
    _(LB, T_SET, "[") \
    _(RB, T_SET, "]") \
    _(LC, T_SET, "{") \
    _(RC, T_SET, "}") \
    _(STRUCT, T_SET, "struct") \
    _(RETURN, T_SET, "return") \
    _(IF, T_SET, "if") \
    _(ELSE, T_SET, "else") \
    _(WHILE, T_SET, "while") \
    _(TYPE, T_SET, "type") \
    _(RELOP, T_SET, "RELOP") \
    _(ID, T_SET, "ID") \
/*}}}*/

typedef enum{
#define synt_t_enum(x, is_ter_sym, str) x,
    AllSyntaxSymbol(synt_t_enum)
} synt_t;

typedef union {
    int cnt_int;
    float cnt_flt;
    const char* id_lit;
} value_t;


#ifdef __cplusplus
extern "C" {
#endif
int new_leaf(synt_t synt_sym, value_t attrib);
int new_node(synt_t synt_sym, int cld_nr, ...);
void set_root(int node_idx);
void print_from_root();
#ifdef __cplusplus
}
#endif
#endif
