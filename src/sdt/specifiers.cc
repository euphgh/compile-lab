#include "ast.h"
#include "sdt.hh"
#include <string>
using std::string;
extern bool struct_def;
const type_t* Specifier_c(const node_t& node) {
    return node.child(0).synt_sym == StructSpecifier
               ? StructSpecifier_c(node.child(0))
               : g_type_tbl.find(node.child(0).attrib.id_lit);
}

const type_t* StructSpecifier_c(const node_t& node) {
    auto child1 = node.child(1);
    if (child1.synt_sym == Tag) {
        auto find_res = g_type_tbl.find(Tag_c(child1));
        if (find_res == nullptr) {
            Error17(node.line, Tag_c(child1));
            return g_type_tbl.undefined_type();
        }
        return find_res;
    } else {
        string opt_id;
        if (child1.synt_sym==OptTag){
            auto new_tag = g_type_tbl.find(OptTag_c(child1));
            if (new_tag)
                Error16(node.line, OptTag_c(child1));
            return new_tag;
        }
        else opt_id = g_type_tbl.unique_type_id();
        var_table fields{};
        def_env.def_table = &fields;
        def_env.def_scope = env_t::STRUCT_ENV;
        DefList_c(node.child(3));
        def_env.def_scope = env_t::NONE_ENV;
        def_env.def_table = nullptr;
        return g_type_tbl.insert_ret(type_t{opt_id, fields});
    }
}

string OptTag_c(const node_t& node) {
    return node.child(0).attrib.id_lit;
}

string Tag_c(const node_t& node) {
    return node.child(0).attrib.id_lit;
}
