#include "ast.h"
#include "ast.hh"
#include "debug.h"
#include "sdt.hh"
#include <memory>
#include <fmt/core.h>

func_t* func_env = nullptr;
compst_node* compst_env = nullptr;
env_t def_env = {
    .def_scope = env_t::NONE_ENV,
    .def_table = nullptr,
};

std::unique_ptr<hitIR> CompSt_c(const node_t& node) {
    auto code = std::make_unique<hitIR>();
    switch (node.child(1).synt_sym) {
        case DefList:
            def_env.def_table = &compst_env->vars;
            def_env.def_scope = env_t::COMPST_ENV;
            code ->append(DefList_c(node.child(1)));
            def_env.def_scope = env_t::NONE_ENV;
            def_env.def_table = nullptr;
            if (node.child(2).synt_sym==StmtList)
                code->append(StmtList_c(node.child(2)));
            break;
        case StmtList:
            code->append(StmtList_c(node.child(1)));
            break;
        case RC:
            break;
        default:
            Assert(0, "not expect synt_sym {} from CompSt node {}'s child 1",
                    node.child(1).synt_sym, node.self_idx);
            break;
    }
    return code;
}

std::unique_ptr<hitIR> StmtList_c(const node_t& node) {
    auto stmt_code = Stmt_c(node.child(0));
    if (node.cld_nr == 2)
        stmt_code->append(StmtList_c(node.child(1)));
    return stmt_code;
}

std::unique_ptr<hitIR> Stmt_c(const node_t& node) {
    auto code = std::make_unique<hitIR>();
    switch (node.child(0).synt_sym) {
    case Exp: {
        const type_t* ret_type;
        code->append(Exp_c(node.child(0), reg_t::new_unique(), ret_type));
        break;
    }
    case CompSt: {
        compst_node* last_env = compst_env;
        compst_env = compst_env->add_child(var_table{});
        code->append(CompSt_c(node.child(0)));
        compst_env = last_env;
        break;
    }
    case IF: {
        auto b_true = label_t::new_label();
        auto b_false = label_t::new_label();
        code->append(Cond_c(node.child(2), b_true, b_false));
        code->append(b_true->ir_mark());
        code->append(Stmt_c(node.child(4)));
        if (node.cld_nr == 7) {
            auto next = label_t::new_label();
            code->append(next->ir_goto());
            code->append(b_false->ir_mark());
            code->append(Stmt_c(node.child(6)));
            code->append(next->ir_mark());
        } else
            code->append(b_false->ir_mark());
        break;
    }
    case WHILE: {
        auto cal_cond = label_t::new_label();
        auto b_true = label_t::new_label();
        auto next = label_t::new_label();
        code->append(cal_cond->ir_mark());
        code->append(Cond_c(node.child(2), b_true, next));
        code->append(b_true->ir_mark())->append(Stmt_c(node.child(4)));
        code->append(cal_cond->ir_goto())->append(next->ir_mark());
        break;
    }
    case RETURN: {
        reg_t* ret_reg = reg_t::new_unique();
        const type_t* ret_type{};
        code->append(Exp_c(node.child(1), ret_reg, ret_type));
        if (func_env->ret_type->not_match(ret_type)) {
            Error8(node.line, ret_type->name, func_env->ret_type->name);
        }
        code->append(ret_reg->ret());
        break;
    }
    default:
        Assert(0, "not expect synt_sym {} from Stmt node {}'s child 0",
            node_t::id_to_str.at(node.child(0).synt_sym).first, node.self_idx);
    }
    return code;
}
