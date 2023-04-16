#include "ast.h"
#include "debug.h"
#include "sdt.hh"
#include <memory>
#include <fmt/core.h>
using std::make_unique;
using std::unique_ptr;

unique_ptr<hitIR> Program_c(const node_t& root) { return ExtDefList_c(root.child(0)); }

unique_ptr<hitIR> ExtDefList_c(const node_t& node) {
    unique_ptr<hitIR> code = ExtDef_c(node.child(0));
    if (node.cld_nr > 1)
        code->append(ExtDefList_c(node.child(1)));
    return code;
}

std::unique_ptr<hitIR> ExtDef_c(const node_t& node) {
    auto code = make_unique<hitIR>();
    switch (node.child(1).synt_sym) {
    case ExtDecList: {
        auto base_type = Specifier_c(node.child(0));
        if (g_type_tbl.find(base_type->name)) 
            ExtDecList_c(node.child(1), base_type);
        else Error17(node.line, base_type->name);
        break;
    }
    case SEMI: {
        Specifier_c(node.child(0));
        break;
    }
    case FunDec: {
        auto base_type = Specifier_c(node.child(0));
        if (g_type_tbl.find(base_type->name)) {
            func_env = FunDec_c(node.child(1), base_type);
            code->append(func_env->def_func());
            compst_env = compst_node::new_root(var_table {});
            func_env->compst_tree = compst_env;
            code->append(CompSt_c(node.child(2)));
        }
        else Error17(node.line, base_type->name);
        break;
    }
    default:
        Assert(0, "not expect synt_sym {} from ExtDef node {}'s child 1",
               node.child(1).synt_sym, node.self_idx);
    }
    return code;
}

void ExtDecList_c(const node_t& node, const type_t* inh_type){
    if (g_var_tbl.insert_check(VarDec_c(node.child(0), inh_type)))
        Error3(node.line, node.child(0).attrib.id_lit);
    if (node.cld_nr==3)
        ExtDecList_c(node.child(2), inh_type);
}
