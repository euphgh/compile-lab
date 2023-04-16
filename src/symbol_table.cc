#include "symbol_table.hh"
#include "debug.h"
#include <fmt/core.h>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
using fmt::format;
using std::make_unique;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;
var_table g_var_tbl{};
type_table g_type_tbl{};
func_table g_func_tbl{};
unsigned label_t::total = 0;
unsigned mem_t::total = 0;
unsigned reg_t::total = 0;
std::vector<label_t> label_t::label_pool;
const var_t* var_table::find(std::string id) const {
    for (auto ele = var_list.cbegin(); ele != var_list.cend(); ++ele)
        if (id == ele->name)
            return &*ele;
    return nullptr;
}
void var_table::insert(const var_t item) {
    var_list.push_back(item);
    offset_list.push_back(start_offset);
    start_offset += item.type->size;
}
var_table::var_table()
    : start_offset(0), undefined(var_t{
                           .name = "not define var",
                           .type = g_type_tbl.undefined_type(),
                           .addr = new mem_t(0, ~0),
                       }) {}

bool var_table::insert_check(var_t item) {
    bool exist = find(item.name) == nullptr;
    if (exist == false)
        insert(item);
    return exist == false;
}

const var_t* var_table::insert_ret(var_t item) {
    var_list.push_back(item);
    return &*(std::prev(var_list.end()));
}
unsigned var_table::offset_of(std::string name) const {
    for (unsigned i = 0; i <= var_list.size(); i++)
        if (var_list[i].name == name)
            return offset_list[i];
    return ~0;
};

void var_table::log(unsigned indent) {
    for (auto var : var_list) {
        for (size_t i = 0; i < indent; i++)
            fmt::print("  ");
        fmt::print("{} {};\n", var.type->name, var.name);
    }
}

type_table::type_table() : undefined(type_t{"undefined type"}) {
    type_list.push_back(type_t{"int", 0, 4});
    type_list.push_back(type_t{"float", 0, 4});
}
const type_t* type_table::undefined_type() const { return &undefined; }

type_t::type_t(std::string _name, unsigned _upper, unsigned _size)
    : name(_name), sub(), upper(_upper), size(_size) {}
type_t::type_t(const type_t* sub_type, unsigned _unpper)
    : name(sub_type->name + "[" + std::to_string(_unpper) + "]"),
      sub(var_table{}), upper(_unpper), size((_unpper + 1) * sub_type->size) {}
type_t::type_t(std::string id, var_table _sub)
    : name(id), sub(_sub), upper(0), size(_sub.size()) {}
bool type_t::not_match(const type_t* other) const {
    return name != other->name && this->name != "undefined type" &&
           other->name != "undefined type";
}
string type_t::base_name() const {
    return upper == 0 ? name : name.substr(0, name.find_last_of('['));
}
unsigned type_t::base_size() const {
    return upper == 0 ? size : size / (upper + 1);
}

const type_t* type_table::find(std::string id) const {
    for (auto ele = type_list.cbegin(); ele != type_list.cend(); ++ele)
        if (id == ele->name)
            return &*ele;
    return nullptr;
}
void type_table::insert(type_t item) { type_list.push_back(item); }
const type_t* type_table::insert_ret(type_t item) {
    type_list.push_back(item);
    return &*(std::prev(type_list.end()));
}
unsigned type_table::total_anonymous = 0;
string type_table::unique_type_id() {
    return "anonymous struct" + std::to_string(total_anonymous++);
}

func_table::func_table() : undefined(new func_t{"undefined func"}) {}
const func_t* func_table::find(std::string id) const {
    for (auto ele = func_list.cbegin(); ele != func_list.cend(); ++ele)
        if (id == ele->name)
            return &*ele;
    return nullptr;
}
void func_table::insert(func_t item) { func_list.push_back(item); }
bool func_t::param_match(
    const std::vector<const type_t*> param_type_list) const {
    const vector<var_t>& def_vct = params.var_list;
    for (unsigned idx; idx < def_vct.size(); idx++) {
        if (def_vct[idx].type->not_match(param_type_list[idx]))
            return false;
    }
    return true;
}

func_t* func_table::insert_ret(func_t item) {
    func_list.push_back(item);
    return &*(std::prev(func_list.end()));
}

std::string func_t::to_string() const {
    stringstream buffer(name);
    buffer << "(";
    for (auto var : params.var_list)
        buffer << var.type->name << ",";
    buffer << ")";
    return buffer.str();
}
const var_t* func_t::find_param(std::string id) const {
    return params.find(id);
}
unique_ptr<hitIR> func_t::def_func() const {
    return make_unique<hitIR>(format("FUCTION {} :", name));
}

compst_node::compst_node(var_table _vars, compst_node* _parent)
    : vars(_vars), parent(_parent) {}
compst_node* compst_node::new_root(var_table root_vars) {
    return new compst_node(root_vars);
}

compst_node* compst_node::add_child(var_table child_vars) {
    sub.push_back(new compst_node{child_vars});
    return *std::prev(sub.end());
}
void compst_node::log() const { recurse_print(0); }
void compst_node::recurse_print(unsigned level) const {
    for (auto son : sub) {
        if (son->sub.size() > 0)
            son->recurse_print(level + 1);
        else {
            for (size_t i = 0; i < level; i++)
                fmt::print("  ");
            fmt::print("{{\n");
            son->vars.log((level + 1) * 2);
            for (size_t i = 0; i < level; i++)
                fmt::print("  ");
            fmt::print("}}\n");
        }
    }
}
