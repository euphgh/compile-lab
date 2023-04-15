#include "symbol_table.hh"
#include <fmt/core.h>
#include <sstream>
#include <string>
#include "debug.h"
using std::string;
using std::vector;
using std::stringstream;
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
var_table::var_table() : start_offset(0) {
    undefined = new var_t{
        .name = "not define var",
        .type = nullptr,
        .ir = { .mem = nullptr, },
    };
}
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
void var_table::log(unsigned indent){
    for (auto var : var_list) {
        for (size_t i = 0; i < indent; i++) 
            fmt::print("  ");
        fmt::print("{} {};\n",var.type->name, var.name);
    }
}

type_table::type_table(): undefined (new type_t {"undefined type"}) {}
type_t::type_t(string name) : name(""), sub(var_table{}), upper(0), size(0) {}
type_t::type_t(const type_t* sub_type, unsigned _unpper)
    : name(sub_type->name + "[" + std::to_string(_unpper) + "]"),
      sub(var_table{}), upper(_unpper), size((_unpper + 1) * sub_type->size) {}
type_t::type_t(std::string id, var_table _sub)
    : name(id), sub(_sub), upper(0), size(_sub.size()) {}

const type_t* type_table::find(std::string id) const {
    for (auto ele = type_list.cbegin(); ele != type_list.cend(); ++ele)
        if (id == ele->name)
            return &*ele;
    return nullptr;
}
void type_table::insert(type_t item) { type_list.push_back(item); }

func_table::func_table(): undefined(new func_t {"undefined func"}) {}
const func_t* func_table::find(std::string id) const {
    for (auto ele = func_list.cbegin(); ele != func_list.cend(); ++ele)
        if (id == ele->name)
            return &*ele;
    return nullptr;
}
void func_table::insert(func_t item) { func_list.push_back(item); }
bool func_t::param_match(const std::vector<const type_t*> param_type_list) const{
    const vector<var_t>& def_vct = params.var_list; 
    for (unsigned idx; idx < def_vct.size(); idx++) {
        if (def_vct[idx].type->not_match(param_type_list[idx]))
            return false;
    }
    return true;
}

func_t::func_t(string _name, var_table _param, compst_node* _compst_tree): 
name(_name), params(_param), compst_tree(_compst_tree) {}

hitIR func_t::call() const{ TODO(); }

std::string func_t::to_string() const{
    stringstream buffer(name);
    buffer << "(";
    for (auto var : params.var_list)
        buffer << var.type->name << ",";
    buffer << ")";
    return buffer.str();
}
const var_t* func_t::find_param(std::string id) const{ return params.find(id); }

compst_node::compst_node(var_table _vars, compst_node* _parent):
    vars(_vars), parent(_parent) {}
compst_node* compst_node::new_root(var_table root_vars){
    return new compst_node(root_vars);
}
void compst_node::log() const{
    recurse_print(0);
}
void compst_node::recurse_print(unsigned level) const {
    for (auto son : sub) {
        if (son->sub.size() > 0) son->recurse_print(level+1);
        else {
            for (size_t i = 0; i < level; i++) 
                fmt::print("  ");
            fmt::print("{{\n");
            son->vars.log((level+1)*2);
            for (size_t i = 0; i < level; i++) 
                fmt::print("  ");
            fmt::print("}}\n");
        }
    }
}
