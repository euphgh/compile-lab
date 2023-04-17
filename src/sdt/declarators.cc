#include "debug.h"
#include "sdt.hh"
#include <csignal>
#include <deque>
#include <fmt/core.h>
using std::string;
const var_t VarDec_c(const node_t& node, const type_t* inh_type) {
    if (node.child_synt(0) == ID) {
        string id_name = node.child(0).attrib.id_lit;
        return var_t{.name = id_name, .type = inh_type};
    } else {
        const var_t sub_var = VarDec_c(node.child(0), inh_type);
        unsigned array_num = node.child(2).attrib.cnt_int;
        return var_t{
            .name = sub_var.name,
            .type = g_type_tbl.insert_ret(type_t{sub_var.type, array_num}),
        };
    }
}

func_t* FunDec_c(const node_t& node, const type_t* inh_ret_type) {
    string func_name = node.child(0).attrib.id_lit;
    if (g_func_tbl.find(func_name) != nullptr)
        Error4(node.line, func_name);
    func_t* func_p;
    if (node.cld_nr == 3) { // no para
        func_p =
            g_func_tbl.insert_ret(func_t{func_name, inh_ret_type, var_table{}});
    } else {
        func_p = g_func_tbl.insert_ret(func_t{
            func_name,
            inh_ret_type,
            VarList_c(node.child(2)),
        });
    }
    return func_p;
}

var_table VarList_c(const node_t& node) {
    var_t para = ParamDec_c(node.child(0));
    var_table res = (node.cld_nr == 1) ? var_table{} : VarList_c(node.child(2));
    res.insert(para);
    return res;
}

var_t ParamDec_c(const node_t& node) {
    return VarDec_c(node.child(1), Specifier_c(node.child(0)));
}
