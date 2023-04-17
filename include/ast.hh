#ifndef __AST_HH__
#define __AST_HH__

extern "C" {
#include "ast.h"
}

#include <vector>
#include <map>
#include <string>
class node_t {
    // first: is empty/available
    static std::vector<std::pair<bool, node_t>> node_space;

    public:
    static const std::map<synt_t, std::pair<std::string, bool>> id_to_str ;
    int par_idx;
    int cld_idx[8];
    int cld_nr;
    int self_idx;
    unsigned line;
    synt_t synt_sym;
    value_t attrib;
    const node_t& child (int n) const;
    inline synt_t child_synt (int n) const { 
        return n < cld_nr ? child(n).synt_sym : NON; 
    }
    std::string to_string () const;

    static int root_idx;
    static bool has_root;
    static int new_space();
    static bool delete_space(int idx);
    static node_t& check_new_node(int idx);
    static const node_t& get_node(int index);
    static void print_from(int idx, int level);
    static void print_node(node_t& node, int level);
};

#endif /* __AST_H__ */
