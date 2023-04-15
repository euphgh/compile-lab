#include "ast.h"
#include <fmt/core.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
struct var_t;
struct type_t;
class func_t;
class mem_t;
class reg_t;
class label_t;

class hitIR {
  public:
    std::vector<std::string> ir_list;
    /* append code into this and return self, like list */
    hitIR(std::string inst) : ir_list(1, inst) {}
    hitIR() {}
    hitIR* append(std::unique_ptr<hitIR> code);
};

class reg_t {
    static unsigned total;
    unsigned id;
    static std::vector<reg_t> reg_pool;
  public:
    reg_t(unsigned _id) : id(_id) {}
    static reg_t* new_unique();
    static reg_t* define_var(const var_t* basic_var);
    hitIR assign(const reg_t* right) const;
    hitIR assign(int data) const;
    hitIR assign(float data) const;
    enum op_t { op_add, op_sub, op_mul, op_div };
    hitIR is_op_of(op_t op, reg_t* src1, reg_t* src2) const;

    hitIR is_op_of(op_t op, int src1, reg_t* src2) const;
    hitIR is_op_of(op_t op, float src1, reg_t* src2) const;

    hitIR is_op_of(op_t op, reg_t* src1, int src2) const;
    hitIR is_op_of(op_t op, reg_t* src1, float src2) const;

    hitIR is_op_of(op_t op, int src1, int src2) const;
    hitIR is_op_of(op_t op, float src1, float src2) const;

    hitIR if_goto(std::string relop, const reg_t* src2,
                  const label_t* b_true) const;
    hitIR if_goto(std::string relop, int src2, const label_t* b_true) const;

    hitIR is_addr_of(const mem_t* src1) const;
    hitIR load_from(reg_t* src1) const;
    hitIR store_to(reg_t* src1) const;
};
class mem_t {
    static std::vector<mem_t> mem_pool;
    static unsigned total;
  public:
    static std::unique_ptr<hitIR> dec(const var_t* derived_var,
                                      unsigned _size); // on stack not heap
    static const mem_t* map_var(std::string _name);
    mem_t(std::string _name, unsigned _size, unsigned _id);
    unsigned id;
    std::string name;
    unsigned size;
};
class label_t {
    static unsigned total;
    static std::vector<label_t> label_pool;

  public:
    static const label_t* new_label();
    unsigned id;
    label_t(unsigned _id) : id(_id) {}
    std::unique_ptr<hitIR> ir_goto() const;
    std::unique_ptr<hitIR> ir_mark() const;
};

class var_table {
    unsigned start_offset;
    std::vector<unsigned> offset_list;
    const var_t* undefined;

  public:
    std::vector<var_t> var_list;
    var_table();
    const var_t* find(std::string id) const;
    /// only push back
    void insert(const var_t item);
    /// check has or not before insert
    bool insert_check(var_t item);
    /// only insert and return point
    const var_t* insert_ret(var_t item);
    unsigned offset_of(std::string name) const;
    inline const var_t* undefined_var() const { return undefined; }
    inline unsigned size() { return start_offset; }
    inline bool empty() const { return offset_list.size() == 1; }
    void log(unsigned indent);
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

    type_t(std::string name = "");
    type_t(const type_t* sub_type, unsigned _unpper); // array
    type_t(std::string id, var_table _sub);
    inline bool is_int() const { return name == "int"; }
    inline bool is_float() const { return name == "float"; }
    inline bool is_basic() const { return is_int() || is_float(); }
    inline bool not_basic() const { return is_basic() == false; }
    inline bool is_array() const { return upper > 0; }
    inline bool is_struct() const { return sub.empty() == false; }
    inline unsigned base_size() const {
        return upper == 0 ? size : size / (upper + 1);
    }
    inline std::string base_name() const {
        return upper == 0 ? name : name.substr(0, name.find_last_of('['));
    }
    // TODO: not impliment
    bool not_match(const type_t* other) const;
};

class type_table {
    std::vector<type_t> type_list;
    const type_t* undefined;

  public:
    type_table();
    const type_t* find(std::string id) const;
    void insert(type_t item);
    inline const type_t* undefined_type() const { return undefined; }
};
extern type_table g_type_tbl;

class func_table {
    std::vector<func_t> func_list;
    const func_t* undefined;

  public:
    func_table();
    const func_t* find(std::string id) const;
    void insert(func_t item);
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
    compst_node(var_table _vars = var_table{}, compst_node* _parent = nullptr);
    compst_node* add_child(var_table child_vars);
    inline bool is_root() const { return parent == nullptr; }
    inline compst_node* up() const { return parent; }
    inline compst_node* child(unsigned num) const { return sub.at(num); }
    inline const std::vector<compst_node*>& children() const { return sub; }
    inline const var_t* find(std::string name) { return vars.find(name); }
    void log() const;
    void recurse_print(unsigned level) const;
};

class func_t {
    const var_table params;
    compst_node* compst_tree;

  public:
    std::string name;
    func_t(std::string _name = "", var_table _param = var_table{},
           compst_node* _compst_tree = nullptr);
    const type_t* ret_type;
    bool param_match(const std::vector<const type_t*> param_type_list) const;
    hitIR call() const; // TODO:
    std::string to_string() const;
    const var_t* find_param(std::string id) const;
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
