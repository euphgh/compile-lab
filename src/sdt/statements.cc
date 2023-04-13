#include "ast.h"
#include "debug.h"
#include "sdt.hh"

hitIR CompSt_c(const node_t& node) {
    def_env.def_table = &compst_env->vars;
    def_env.def_scope = env_t::COMPST_ENV;
    hitIR def_code = DefList_c(node.child(1));
    def_env.def_scope = env_t::NONE_ENV;
    def_env.def_table = nullptr;
    hitIR stmt_code = StmtList_c(node.child(2));
    return def_code.append(stmt_code);
}

hitIR StmtList_c(const node_t& node) {
    hitIR stmt_code = Stmt_c(node.child(0));
    if (node.cld_nr == 2)
        stmt_code.append(StmtList_c(node.child(1)));
    return stmt_code;
}

hitIR Stmt_c(const node_t& node) {
    hitIR code;
    switch (node.child(0).synt_sym) {
    case Exp: {
        code = Exp_c(node.child(0), nullptr, nullptr);
        break;
    }
    case CompSt: {
        compst_env = compst_env->add_child(var_table{});
        code = CompSt_c(node.child(0));
        break;
    }
    case IF: {
        auto b_true = label_t::new_label();
        auto b_false = label_t::new_label();
        code = Cond_c(node.child(2), b_true, b_false);
        code.append(b_true->ir_mark());
        code.append(Stmt_c(node.child(4)));
        if (node.cld_nr == 7) {
            auto next = label_t::new_label();
            code.append(next->ir_goto());
            code.append(b_false->ir_mark());
            code.append(Stmt_c(node.child(6)));
            code.append(next->ir_mark());
        } else
            code.append(b_false->ir_mark());
        break;
    }
    case WHILE: {
        auto cal_cond = label_t::new_label();
        auto b_true = label_t::new_label();
        auto next = label_t::new_label();
        code = cal_cond->ir_mark();
        code.append(Cond_c(node.child(2), b_true, next));
        code.append(b_true->ir_mark()).append(Stmt_c(node.child(4)));
        code.append(cal_cond->ir_goto()).append(next->ir_mark());
        break;
    }
    default:
        Assert(0, "not expect synt_sym {} from Stmt node {}'s child 0",
               node.child(1).synt_sym, node.self_idx);
    }
    return code;
}
