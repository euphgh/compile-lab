#include "ast.h"
#include "sdt.hh"

/** insert type to global type table
 * return a point to it
 */
const type_t* Specifier_c(const node_t& node) {
    return node.child_n(0).synt_sym == StructSpecifier
               ? StructSpecifier_c(node.child_n(0))
               : g_type_tbl.find(node.child_n(0).attrib.id_lit);
}

/** StructSpecifier has two option
 * insert struct to global type table when def
 * check global type table when use
 * return a point to it
 * Error16: id is not same with struct table and global var
 */
const type_t* StructSpecifier_c(const node_t& node) {
    auto child1 = node.child_n(1);
    if (child1.synt_sym == Tag) {
        auto find_res = g_type_tbl.find(child1.attrib.id_lit);
        if (find_res == nullptr) {
            Error16(node.line, child1.attrib.id_lit);
            return g_type_tbl.find("any");
        }
        return find_res;
    } else {
        string opt_id = child1.synt_sym == OptTag ? OptTag_c(child1)
                                                  : g_type_tbl.unique_id();
        def_env = STRUCT_ENV;
        var_table fields{};
        struct_env = &fields;
        hitIR def = DefList_c(node.child_n(3));
        def_env = NONE_ENV;
        struct_env = nullptr;
        return g_type_tbl.insert_ret(type_t{opt_id, fields});
    }
}

/** return unique id if empty
 */
string OptTag_c(const node_t& node) {
    return node.child_n(0).attrib.id_lit;
}

/** return struct id
 */
string Tag_c(const node_t& node) {
    return node.child_n(0).attrib.id_lit;
}
