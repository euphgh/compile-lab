#include "ast.h"
#include <fmt/core.h>
#include <map>
#include <string>
#include <vector>
struct var_t;
struct type_t;
class func_t;
class mem_t;
class reg_t;
class label_t;

class hitIR {
    std::string code;

  public:
    /* append code into this and return self, like list */
    hitIR append(hitIR code);
    /* add code with this and return new hitIR */
    hitIR plus(hitIR code) const;
};

class reg_t {
  public:
    static reg_t* new_unique();
    static reg_t* map_var();
    static reg_t* define_var(const var_t* basic_var);
    hitIR assign(const reg_t* right) const ;
    hitIR assign(int data) const ;
    hitIR assign(float data) const ;
    enum op_t{
        op_add,
        op_sub,
        op_mul,
        op_div
    };
    hitIR is_op_of(op_t op, reg_t* src1, reg_t* src2) const;

    hitIR is_op_of(op_t op, int    src1, reg_t* src2) const;
    hitIR is_op_of(op_t op, float  src1, reg_t* src2) const;

    hitIR is_op_of(op_t op, reg_t* src1, int    src2) const;
    hitIR is_op_of(op_t op, reg_t* src1, float  src2) const;

    hitIR is_op_of(op_t op, int    src1, int    src2) const;
    hitIR is_op_of(op_t op, float  src1, float  src2) const;

    hitIR if_goto(std::string relop, const reg_t* src2, const label_t* b_true) const;
    hitIR if_goto(std::string relop, int src2, const label_t* b_true) const;

    hitIR is_addr_of(const mem_t* src1) const;
    hitIR load_from (reg_t* src1) const;
    hitIR store_to  (reg_t* src1) const;
};
class mem_t {
  public:
    static mem_t* dec(const var_t* derived_var, unsigned size); // on stack not heap
    static mem_t* map_var(std::string name);
};
class label_t {
  public:
    static const label_t* new_label();
    hitIR ir_goto() const;
    hitIR ir_mark() const;
};



template <class T> class sym_table {
    std::map<std::string, T> table;

  public:
    T* find(std::string id) const;
    void insert(T item);
    T* insert_ret(T item);
    bool append(sym_table table);
    static std::string unique_id();
};

class var_table : public sym_table<var_t> {
  public:
    var_table();
    bool check_and_insert(var_t item);
    unsigned offset_of(std::string name) const;
    const var_t* not_define_var() const;
};
extern var_table g_var_tbl;

class type_t {
    /*
     * basic data type is like follow
     * name = int, float
     * sub = null
     * upper = 0
     * size = 4
     *
     * array data type is like follow
     * name = id/int/float
     * sub =  null
     * upper = positive number
     * size = (upper+1) * type_table.find(name).size
     *
     * struct data type is like follow
     * name = id
     * sub  = point to var_table
     * upper = 0
     * size = sum(filter(lambda type: type.size, sub))
     */
  public:
    std::string name;
    var_table sub;
    unsigned upper;
    unsigned size;

    type_t();
    type_t(const type_t* sub_type, unsigned _unpper); // array
    type_t(std::string id, var_table _sub);
    bool is_int() const;
    bool is_float() const;
    bool is_basic() const;
    bool not_basic() const;
    bool is_array() const;
    bool is_struct() const;
    unsigned base_size() const;
    std::string base_name() const;
    bool not_match(type_t* other) const;
    std::string to_string() const;
};

class type_table : public sym_table<type_t> {
  public:
    const type_t* undefined_type();
};
extern type_table g_type_tbl;

class func_table : public sym_table<func_t> {
  public:
    const func_t* undefined_func() const;
};
extern func_table g_func_tbl;

struct var_t {
    std::string name;
    const type_t* type;
    // if basic type mem=null, reg point to instance
    // else reg=null, mem point to instance
    union {
        const reg_t* reg;
        const mem_t* mem;
    } ir;
};

struct compst_node {
    var_table vars;
    compst_node* parent;
    std::vector<compst_node*> sub;

  public:
    static compst_node* new_root(var_table root_vars);
    compst_node* add_child(var_table child_vars);
    bool is_root() const;
    compst_node* up() const;
    compst_node* child(unsigned num) const;
    const std::vector<compst_node*>& children() const;
    const var_t* find(std::string name) const;
    void log() const;
};

class func_t {
    std::string name;
    const var_table para;
    compst_node* compst_tree;

  public:
    const type_t* ret_type;
    bool param_match(const std::vector<const type_t*> param_type_list) const;
    hitIR call() const;
    std::string to_string() const;
    const var_t* find_param(std::string) const;
};

#define E_TABLE(_)                                                             \
    _(1, "Undefined variable \"{}\"")                                          \
    _(2, "Undefined function \"{}\"")                                          \
    _(3, "Redefined variable \"{}\"")                                          \
    _(4, "Redefined function \"{}\"")                                          \
    _(5, "Type mismatched for assignment with left \"{}\" and right \"{}\"")   \
    _(6, "The left-hand side of an assignment must be a variable")             \
    _(7, "Type mismatched for oprand \"{}\", operator \"{}\", oprand\"{}\"")   \
    _(8, "Type mismatched for return \"{}\" with def type \"{}\"")             \
    _(9, "Function \"{}\" is not applicable for arguments \"({})\"")           \
    _(10, "\"{}\" is not a array")                                             \
    _(11, "\"{}\" is not a function")                                          \
    _(12, "\"{}\" is not a integer")                                           \
    _(13, "Illegal use of \".\" for Exp\"{}\"")                                \
    _(14, "Non-existent field \"{}\" in struct \"{}\"")                        \
    _(15, "Redefined or define with inital field \"{}\"")                      \
    _(16, "Duplicated name \"{}\"")                                            \
    _(17, "Undefined structure \"{}\"")

#define __def_macro_error__(num, fmtstr)                                       \
    template <typename... param>                                               \
    void Error##num(unsigned line, param&&... params) {                        \
        std::string msg =                                                      \
            fmt::vformat(fmtstr, fmt::make_format_args(params...));            \
        fmt::print("Error type {} at line {}: {:s}", num, line, msg);          \
    }

E_TABLE(__def_macro_error__)
