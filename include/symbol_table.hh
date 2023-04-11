#include "ast.h"
#include <fmt/core.h>
#include <map>
#include <stack>
#include <string>
#include <vector>
struct var_t;
struct type_t;
struct func_t;
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
};
extern var_table g_var_tbl;
extern std::stack<var_table> l_var_tbl;

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

    type_t(const type_t* sub_type, unsigned _unpper); // array
    type_t(std::string id, var_table _sub);
    bool is_int();
    bool is_float();
    bool is_basic();
    bool is_array();
    bool is_struct();
};

class type_table : public sym_table<type_t> {};
extern type_table g_type_tbl;

class func_table : public sym_table<func_t> {};
extern func_table g_func_tbl;

struct var_t {
    std::string name;
    const type_t* type;
};

struct func_t {
    std::string name;
    const type_t* ret_type; // point to a element in type_table
    var_table para;         // instance, not point
};

#define E_TABLE(_)                                                             \
    _(1, "Undefined variable \"{}\"")                                          \
    _(2, "Undefined function \"{}\"")                                          \
    _(3, "Redefined variable \"{}\"")                                          \
    _(4, "Redefined function \"{}\"")                                          \
    _(5, "Type mismatched for assignment with left \"{}\" and right \"{}\"")   \
    _(6, "The left-hand side of an assignment must be a variable")             \
    _(7, "Type mismatched for operands \"{}\" with type\"{}\"")                \
    _(8, "Type mismatched for return \"{}\" with def type \"{}\"")             \
    _(9, "Function \"{}\" is not applicable for arguments \"{}\"")             \
    _(10, "\"{}\" is not a array")                                             \
    _(11, "\"{}\" is not a function")                                          \
    _(12, "\"{}\" is not a integer")                                           \
    _(13, "Illegal use of \".\"")                                              \
    _(14, "Non-existent field \"{}\"")                                         \
    _(15, "Redefined field \"{}\"")                                            \
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
