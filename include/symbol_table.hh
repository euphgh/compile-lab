#include "ast.h"
#include <fmt/core.h>
#include <map>
#include <string>
#include <vector>
struct var_t;
struct type_t;
class func_t;
class hitIR {
    std::string code;

  public:
    /* append code into this and return self, like list */
    hitIR append(hitIR code);
    /* add code with this and return new hitIR */
    hitIR plus(hitIR code) const;
};

template <class T> class sym_table {
    std::map<std::string, T> table;

  public:
    T* find(std::string id);
    void insert(T item);
    T* insert_ret(T item);
    bool append(sym_table table);
    static std::string unique_id();
};

class var_table : public sym_table<var_t> {
  public:
    var_table();
    bool check_and_insert(var_t item);
    unsigned offset_of(std::string name);
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
    const func_t* undefined_func();
};
extern func_table g_func_tbl;

struct var_t {
    std::string name;
    const type_t* type;
};

struct compst_node {
    var_table vars;
    compst_node* parent;
    std::vector<compst_node*> sub;

  public:
    static compst_node* new_root(var_table root_vars);
    compst_node* add_child(var_table child_vars);
    void is_root();
    compst_node* up();
    compst_node* child(unsigned num);
    const std::vector<compst_node*>& children();
    void log();
};

class func_t {
    std::string name;
    const type_t* ret_type;
    const var_table para;
    compst_node* compst_tree;
    public:
    bool param_match(const std::vector<const type_t*> param_type_list) const;
    hitIR call() const;
    std::string to_string() const;
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
    _(9, "Function \"{}\" is not applicable for arguments \"({})\"")             \
    _(10, "\"{}\" is not a array")                                             \
    _(11, "\"{}\" is not a function")                                          \
    _(12, "\"{}\" is not a integer")                                           \
    _(13, "Illegal use of \".\"")                                              \
    _(14, "Non-existent field \"{}\"")                                         \
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
