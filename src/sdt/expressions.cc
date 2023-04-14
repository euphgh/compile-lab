#include "debug.h"
#include "sdt.hh"
#include <vector>
#include <numeric>
#define EXPR_ALL(_)                                                            \
    _(Exp, AND, Exp, NON, Exp_AND_Exp)                                         \
    _(Exp, OR, Exp, NON, Exp_OR_Exp)                                           \
    _(Exp, RELOP, Exp, NON, Exp_RELOP_Exp)                                     \
    _(Exp, PLUS, Exp, NON, Exp_PLUS_Exp)                                       \
    _(Exp, MINUS, Exp, NON, Exp_MINUS_Exp)                                     \
    _(Exp, STAR, Exp, NON, Exp_STAR_Exp)                                       \
    _(Exp, DIV, Exp, NON, Exp_DIV_Exp)                                         \
    _(LP, Exp, RP, NON, LP_Exp_RP)                                             \
    _(MINUS, Exp, NON, NON, MINUS_Exp)                                         \
    _(NOT, Exp, NON, NON, NOT_Exp)                                             \
    _(ID, LP, Args, RP, ID_LP_Args_RP)                                         \
    _(ID, LP, RP, NON, ID_LP_RP)                                               \
    _(Exp, LB, Exp, RB, Exp_LB_Exp_RB)                                         \
    _(Exp, DOT, ID, NON, Exp_DOT_ID)                                           \
    _(ID, NON, NON, NON, IS_ID)                                                \
    _(INT, NON, NON, NON, IS_INT)                                              \
    _(FLOAT, NON, NON, NON, IS_FLOAT)
#define JUDGE_EXPR(e0, e1, e2, e3, ans)                                        \
    (node.child_synt(0) == e0 && node.child_synt(1) == e1 &&                   \
     node.child_synt(2) == e2 && node.child_synt(3) == e3)                     \
        ? ans                                                                  \
        :

#define DEF_EXPR_TYPE(e0, e1, e2, e3, ans) ans,

static hitIR triple_reg(reg_t::op_t op, string op_str, const node_t& node,
                        const reg_t* place) {
    hitIR code;
    reg_t* left = reg_t::new_unique();
    type_t *left_type, *right_type;
    reg_t* right = reg_t::new_unique();
    code.append(Exp_c(node.child(0), left, left_type));
    code.append(Exp_c(node.child(2), right, right_type));
    if (left_type->not_match(right_type) || left_type->not_basic() ||
        right_type->not_basic())
        Error7(node.line, left_type->to_string(), op_str,
               right_type->to_string());
    code.append(place->is_op_of(reg_t::op_add, left, right));
    return code;
}
hitIR Exp_c(const node_t& node, const reg_t* place, const type_t* self_ret) {
    enum exp_t {
        EXPR_ALL(DEF_EXPR_TYPE) IS_ERROR,
    } exp_type = EXPR_ALL(JUDGE_EXPR) IS_ERROR;
    hitIR code;
    switch (exp_type) {
    case Exp_PLUS_Exp:
        code = triple_reg(reg_t::op_add, "+", node, place);
        break;
    case Exp_MINUS_Exp:
        code = triple_reg(reg_t::op_sub, "-", node, place);
        break;
    case Exp_STAR_Exp:
        code = triple_reg(reg_t::op_mul, "*", node, place);
        break;
    case Exp_DIV_Exp:
        code = triple_reg(reg_t::op_div, "/", node, place);
        break;
    case LP_Exp_RP:
        Exp_c(node.child(1), place, nullptr);
        break;
    case Exp_DOT_ID: {
                         reg_t* struct_base = reg_t::new_unique();
                         const type_t* struct_ret;
                         code.append(Exp_c(node.child(0), struct_base, struct_ret));
                         if (struct_ret->is_struct()==false)
                             Error13(node.line, node.child(0).to_string());
                         reg_t* offset = reg_t::new_unique();
                         string field_name = node.child(2).attrib.id_lit;
                         const var_table& struct_vars = struct_ret->sub;
                         const var_t* field_var = struct_vars.find(field_name);
                         if (field_var==nullptr){
                             Error14(node.line, field_name, struct_ret->name);
                             break;
                         }
                         code.append(offset->assign((int)struct_vars.
                                     offset_of(field_name)));
                         self_ret = field_var->type;
                         if (self_ret->is_basic()) {
                             reg_t* addr = reg_t::new_unique();
                             code.append(addr->is_op_of(reg_t::op_add, struct_base, offset));
                             code.append(place->load_from(addr));
                         }
                         else code.append(place->is_op_of(reg_t::op_add, struct_base, offset));
                         break;
                     }
    case Exp_LB_Exp_RB: {
                            reg_t* array_base = reg_t::new_unique();
                            const type_t* sub_ret;
                            code.append(Exp_c(node.child(0), array_base, sub_ret));
                            if (sub_ret->is_array()==false)
                                Error10(node.line, node.child(0).to_string());
                            reg_t* index = reg_t::new_unique();
                            code.append(Exp_c(node.child(2), index, sub_ret));
                            if (sub_ret->is_int()==false)
                                Error12(node.line, node.child(2).to_string());
                            reg_t* offset = reg_t::new_unique();
                            code.append(offset->is_op_of(reg_t::op_mul, (int)sub_ret->base_size(), index)) ;
                            code.append(place->is_op_of(reg_t::op_add, array_base, offset));
                            self_ret = g_type_tbl.find(sub_ret->base_name());
                            break;
                        }
    case ID_LP_RP: {
                       const func_t* func = g_func_tbl.find(node.child(0).attrib.id_lit);
                       if (func==nullptr) {
                           Error2(node.line, node.child(0).attrib.id_lit);
                           func =g_func_tbl.undefined_func();
                       }
                       if (func->param_match(vector<const type_t*> {}))
                           code.append(func->call());
                       else Error9(node.line, func->to_string(), "");
                       break;
                   }
    case ID_LP_Args_RP: {
                            const func_t* func = g_func_tbl.find(node.child(0).attrib.id_lit);
                            if (func==nullptr) {
                                Error2(node.line, node.child(0).attrib.id_lit);
                                func =g_func_tbl.undefined_func();
                            }
                            vector<const type_t*> param_type {};
                            code = Args_c(node.child(2), param_type);
                            if (func->param_match(param_type))
                                code.append(func->call());
                            else {
                                using std::accumulate;
                                Error9(node.line, func->to_string(), 
                                        std::accumulate(std::next(param_type.begin()), param_type.end(), 
                                            param_type[0]->to_string(), 
                                        [](string a, const type_t* b){ return std::move(a) + "," + b->to_string();}));
                            }
                            break;
                        }
    case Exp_RELOP_Exp:
    case Exp_AND_Exp:
    case Exp_OR_Exp:
    case NOT_Exp: {
        label_t* b_true = label_t::new_label();
        label_t* b_false = label_t::new_label();
        code.append(place->assign(0));
        code.append(Cond_c(node, b_true, b_false));
        code.append(b_true->ir_mark());
        code.append(place->assign(1));
        code.append(b_false->ir_mark());
        break;
    }
    case MINUS_Exp: {
        type_t* ret_type;
        reg_t* tmp1 = reg_t::new_unique();
        code = Exp_c(node.child(1), tmp1, ret_type);
        code.append(place->is_op_of(reg_t::op_sub, 0, tmp1));
        break;
    }
    case IS_INT:
        code = place->assign(node.attrib.cnt_int);
        break;
    case IS_ID:{
                   string id_name = node.child(0).attrib.id_lit;
                   const var_t* id_var = nullptr;
                   const compst_node* search_env = compst_env;
                   do {
                       id_var = search_env->find(id_name);
                       search_env = search_env->up();
                   } while (search_env!=nullptr && id_var==nullptr);
                   id_var = id_var ? id_var : func_env->find_param(id_name);
                   id_var = id_var ? id_var : g_var_tbl.find(id_name);
                   if (id_var==nullptr) {
                       Error1(node.line, id_name);
                       id_var = g_var_tbl
                   }
               }
    case IS_FLOAT:
        code = place->assign(node.attrib.cnt_flt);
        break;
    case IS_ERROR:
        Assert(0, "not expect synt_sym {} from Exp node {}'s child 0",
               node.child(0).synt_sym, node.self_idx);
    }
    return code;
}

hitIR Args_c(const node_t& node, vector<const type_t*>& param_list) {
    hitIR code;
    return code;
}

hitIR Cond_c(const node_t& node, label_t* b_true, label_t* b_false) {
    hitIR code;
    return code;
}