#include "sdt.hh"
#include "debug.h"
#include <fmt/core.h>
#include <deque>

const var_t VarDec_c(node_t& node, const type_t* inh_type) {
    if (node.synt_sym == ID) {
        string id_name = node.attrib.id_lit;
        return var_t{.name = id_name, .type = inh_type};
    } else {
        const var_t sub_var = VarDec_c(node.child(0), inh_type);
        unsigned array_num = node.child(2).attrib.cnt_int;
        return var_t {
            .name = sub_var.name,
            .type = g_type_tbl.insert_ret(type_t{sub_var.type, array_num}),
        };
    }
}

func_t* FunDec_c(node_t& node, const type_t* inh_ret_type) {
    string func_name = node.child(0).attrib.id_lit;
    func_t* func_p = g_func_tbl.find(func_name);
    if (func_p!=nullptr)
        Error3(node.line, node.attrib.id_lit);
    if (node.cld_nr == 3) { // no para
        func_p = g_func_tbl.insert_ret(func_t{
            .name = func_name,
            .ret_type = inh_ret_type,
            .para = var_table {},
            .compst_tree = nullptr
        });
    } else {
        func_p = g_func_tbl.insert_ret(func_t{
            .name = func_name,
            .ret_type = inh_ret_type,
            .para = VarList_c(node.child(2)),
            .compst_tree = nullptr,
        });
    }
    return func_p;
}

var_table VarList_c(node_t& node) {
    var_table res {};
    if (node.cld_nr==1)
        res.insert(ParamDec_c(node.child(0)));
    else {
        res.insert(ParamDec_c(node.child(0)));
        bool deplicate = res.append(VarList_c(node.child(2)));
        // if (deplicate) Error() no duplicate para
    }
    return res;
}

var_t ParamDec_c(node_t& node) {
    return VarDec_c(node.child(1), 
            Specifier_c(node.child(0)));
}
