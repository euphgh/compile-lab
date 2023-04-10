#include "ast.h"
#include "debug.h"
#include "sdt.hh"
#include <fmt/core.h>
hitIR Program_c(const node_t& root) { return ExtDefList_c(root.child_n(0)); }

hitIR ExtDefList_c(const node_t& node) {
    hitIR code1 = ExtDef_c(node.child_n(0));
    if (node.cld_nr > 1)
        return code1.append(ExtDefList_c(node.child_n(1)));
    else
        return code1;
}

hitIR ExtDef_c(const node_t& node) {
    hitIR ir;
    switch (node.get_node(1).synt_sym) {
    case ExtDecList: {
        auto base_type = Specifier_c(node.child_n(0));
        if (g_type_tbl.find(base_type->name)) 
            ExtDecList_c(node.child_n(1), base_type);
        else
            Error(17, node.line, "Undefined structure {}", base_type->name);
        break;
    }
    case SEMI: {
        Specifier_c(node.child_n(0));
        break;
    }
    case FunDec: {
        auto base_type = Specifier_c(node.child_n(0));
        if (g_type_tbl.find(base_type->name)) 
            ir = FunDec_c(node.child_n(1), base_type);
        else
            Error(17, node.line, "Undefined structure {}", base_type->name);
        break;
    }
    default:
        Assert(0, "not expect synt_sym {} from ExtDef node {}'s child 1",
               node.get_node(1).synt_sym, node.self_idx);
    }
    return ir;
}

void ExtDecList_c(const node_t& node, const type_t* inh_type){
    if (g_var_tbl.check_and_insert(VarDec_c(node.child_n(0), inh_type)))
        Error(3, node.line, "Redefined variable \"{}\"", node.child_n(0).attrib.id_lit);
    if (node.cld_nr==3)
        ExtDecList_c(node.child_n(2), inh_type);
}
