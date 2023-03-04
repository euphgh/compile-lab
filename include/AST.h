#define AllSyntaxSymbol(_) \
    _(Program) _(ExtDefList) _(ExtDef) _(ExtDecList) \
    _(Specifier) _(StructSpecifier) _(OptTag) _(Tag) \
    _(VarDec) _(FunDec) _(VarList) _(ParamDec) \
    _(CompSt) _(StmtList) _(Stmt) \
    _(DefList) _(Def) _(DecList) _(Dec) \
    _(Exp) _(Args) \
    _(INT) _(FLOAT) \
    _(SEMI) _(COMMA) \
    _(ASSIGNOP) \
    _(PLUS) _(MINUS) _(STAR) _(DIV) \
    _(AND)  _(OR) \
    _(DOT) \
    _(NOT) \
    _(LP) _(RP) \
    _(LB) _(RB) \
    _(LC) _(RC) \
    _(STRUCT) \
    _(RETURN) \
    _(IF) _(ELSE) \
    _(WHILE) \
    _(TYPE) \
    _(RELOP) \
    _(ID) \
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
