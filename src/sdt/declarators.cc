#include "sdt.hh"
#include "debug.h"
#include <fmt/core.h>
/* recieve base type from Specifier_c 
 * add array type into global type table 
 * return var_t to ExtDecList, ParamDec or Dec 
 * add var_t to responded var_table */
const var_t VarDec_c(node_t& node, const type_t* inh_type) {
    if (node.synt_sym == ID) {
        string id_name = node.attrib.id_lit;
        return var_t{.name = id_name, .type = inh_type};
    } else {
        const var_t sub_var = VarDec_c(node.child_n(0), inh_type);
        unsigned array_num = node.child_n(2).attrib.cnt_int;
        return var_t {
            .name = sub_var.name,
            .type = g_type_tbl.insert_ret(type_t{sub_var.type, array_num}),
        };
    }
}

/* return a var_t to VarList_c to build a var_table */
var_t ParamDec_c(node_t& node) {
    return VarDec_c(node.child_n(1), 
            Specifier_c(node.child_n(0)));
}

/* return a var_table to FunDec_c to build func_t */
var_table VarList_c(node_t& node) {
    var_table res {};
    if (node.cld_nr==1)
        res.insert(ParamDec_c(node.child_n(0)));
    else {
        res.insert(ParamDec_c(node.child_n(0)));
        bool deplicate = res.append(VarList_c(node.child_n(2)));
        // if (deplicate) Error() no duplicate para
    }
    return res;
}

/* add func_t into a global function table */
void FunDec_c(node_t& node, const type_t* inh_ret_type) {
    string func_name = node.child_n(0).attrib.id_lit;
    if (g_func_tbl.find(func_name))
        Error(3, node.line, "Redefined function \"{}\"", node.attrib.id_lit);
    if (node.cld_nr == 3) { // no para
        g_func_tbl.insert(func_t{
            .name = func_name,
            .ret_type = inh_ret_type,
            .para = var_table{},
        });
    } else {
        var_table para_table = VarList_c(node.child_n(2));
        g_func_tbl.insert(func_t{
            .name = func_name,
            .ret_type = inh_ret_type,
            .para = para_table,
        });
    }
}

var_table ExtDecList_c(node_t& node, const type_t* inh_type);
