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



class type_table : public sym_table<type_t> { };
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

#define Error(No, line, str, ...) \
    print_line_error(No, line, fmt::format(str, ##__VA_ARGS__));
void print_line_error(unsigned NO, unsigned line, std::string msg = "");
