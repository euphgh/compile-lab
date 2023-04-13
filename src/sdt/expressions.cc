#include "debug.h"
#include "sdt.hh"
hitIR Exp_c(const node_t& node, const reg_t* place, type_t* ret_type) {
    hitIR code;
    switch (node.child(0).synt_sym) {
    case Exp: {
        break;
    }
    case MINUS: {
        type_t* ret_type;
        reg_t* tmp1 = reg_t::new_unique();
        code = Exp_c(node.child(1), tmp1, ret_type);
        code.append(place->is_op_of(reg_t::op_sub, 0, tmp1));
        break;
    }
    case INT:
        code = place->assign(node.attrib.cnt_int);
        break;
    case FLOAT:
        code = place->assign(node.attrib.cnt_flt);
        break;
    default:
        Assert(0, "not expect synt_sym {} from Exp node {}'s child 0",
               node.child(0).synt_sym, node.self_idx);
    }
    return code;
}

hitIR Args_c(const node_t& node, vector<mem_t*>& arg_list) {
    hitIR code;
    return code;
}

hitIR Cond_c(const node_t& node, label_t* b_true, label_t* b_false) {
    hitIR code;
    return code;
}
