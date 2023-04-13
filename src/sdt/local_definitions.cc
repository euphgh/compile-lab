#include "debug.h"
#include "sdt.hh"

hitIR DefList_c(const node_t& node) {
    hitIR code = Def_c(node.child(0));
    if (node.cld_nr > 1)
        code.append(DefList_c(node.child(1)));
    return code;
}

hitIR Def_c(const node_t& node) {
    const type_t* base_type = Specifier_c(node.child(0));
    if (g_type_tbl.find(base_type->name) == nullptr) {
        Error17(node.line, base_type->name);
        base_type = g_type_tbl.undefined_type();
    }
    return DecList_c(node.child(1), base_type);
}

hitIR DecList_c(const node_t& node, const type_t* inh_type) {
    hitIR code = Dec_c(node.child(0), inh_type);
    if (node.cld_nr > 1)
        code.append(DecList_c(node.child(2), inh_type));
    return code;
}

hitIR Dec_c(const node_t& node, const type_t* inh_type) {
    hitIR code;
    var_t var = VarDec_c(node.child(0), inh_type);
    var_table* def_tlb = def_env.def_table;
    switch (def_env.def_scope) {
    case env_t::COMPST_ENV: {
        if (def_tlb->find(var.name)) {
            Error3(node.line, var.name);
        } else
            def_tlb->insert(var);
        if (node.cld_nr>1){
            type_t ret_type {};
            code = Exp_c(node.child(2), reg_t::new_unique(), &ret_type);
            if (inh_type->not_match(&ret_type))
                Error5(node.line, inh_type->to_string(),ret_type.to_string());
        }
    }
    case env_t::STRUCT_ENV: {
        if (node.cld_nr>1 || def_tlb->find(var.name)) {
            Error15(node.line, var.name);
        } else
            def_tlb->insert(var);
    }
    case env_t::NONE_ENV: 
        Assert(0, "not expect Dec when def_env==NONE_ENV");
    }
    return code;
}
