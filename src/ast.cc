#include "ast.hh"
#include <vector>
#include <cstdarg>
#include "debug.h"
#include "macro.h"
#include "fmt/core.h"
#include <csignal>
#include <map>
#include <tuple>


/////////////////////////////////////////////////
/// impliment for cpp class
/////////////////////////////////////////////////
#define synt_t_map(x, is_ter_sym, str) std::make_pair(x, std::make_pair(#x, is_ter_sym)),
const std::map<synt_t, std::pair<std::string, bool>> node_t::id_to_str = {
        AllSyntaxSymbol(synt_t_map)
    };
int node_t::root_idx = 0;
bool node_t::has_root = false;
int node_t::new_space(){/*{{{*/
    bool has_space = false;
    int idx = 0;
    for (int i = 0; i<node_space.size(); i++) {
        if (node_space[i].first){
            has_space = true;
            idx = i;
        }
    }
    if (has_space==false){
        idx = node_space.size();
        node_t empty;
        node_space.push_back(std::make_pair(true, empty));
    }
    return idx;
}/*}}}*/
bool node_t::delete_space(int idx){/*{{{*/
    bool is_leaf = node_space.at(idx).second.cld_nr==0;
    bool parent_normal = false;
    if (is_leaf) {
        node_space[idx].first = true;
        int& bor_nr = node_space[node_space[idx].second.par_idx].second.cld_nr;
        int* bor_idxes = node_space[node_space[idx].second.par_idx].second.cld_idx;
        for (size_t i = 0; i < bor_nr; i++) {
            if (parent_normal) bor_idxes[i-1] = bor_idxes[i];
            if (bor_idxes[i]==idx) parent_normal = true;
        }
        if (parent_normal) bor_nr--;
    }
    return is_leaf && parent_normal;
}/*}}}*/
node_t& node_t::check_new_node(int idx){/*{{{*/
    auto& node_pair = node_space.at(idx);
    if (node_pair.first==false){
        std::raise(SIGTRAP);
        Assert(0, "new space return a index %d but it first is true",idx);
    }
    node_pair.first = false;
    return node_pair.second;
}/*}}}*/
void node_t::print_node(node_t& node, int level){/*{{{*/
    for (size_t i = 0; i < level; i++) fmt::print("  ");
    switch (node.synt_sym) {
        case ID:
        case TYPE:
            fmt::print("{}: {}\n", 
                    id_to_str.at(node.synt_sym).first,
                    node.attrib.id_lit);
            break;
        case INT:
            fmt::print("INT: {}\n", node.attrib.cnt_int);
            break;
        case FLOAT:
            fmt::print("FLOAT: {:.6}\n", node.attrib.cnt_flt);
            break;
        default:
            fmt::print("{}\n", id_to_str.at(node.synt_sym).first);
            break;
    }
}/*}}}*/
void node_t::print_from(int idx, int level){/*{{{*/
    auto node = node_t::node_space.at(idx).second;
    Assert(node_t::node_space[idx].first==false, "Traver to a empty node");
    print_node(node, level);
    for (size_t i = 0; i < node.cld_nr; i++) {
        print_from(node.cld_idx[i], level+1);
    }
}/*}}}*/
const node_t& node_t::get_node(int index){/*{{{*/
    auto& [is_empty, node] = node_space.at(index);
    Assert(is_empty==false, "try get a empty node {}", index);
    return node;
}/*}}}*/
const node_t& node_t::child(int n) const {/*{{{*/
    Assert(n < cld_nr, 
            "try get child {} from node {} who has {} child", n, self_idx, cld_nr);
    return get_node(cld_idx[n]);
}/*}}}*/

/////////////////////////////////////////////////
/// impliment for c api
/////////////////////////////////////////////////
int new_leaf(synt_t synt_sym, value_t attrib) {/*{{{*/
    int idx = node_t::new_space();
    node_t& node = node_t::check_new_node(idx);
    node.cld_nr = 0;
    node.synt_sym = synt_sym;
    node.attrib = attrib;
    return idx;
}/*}}}*/
int new_node(synt_t synt_sym, int argc, ...) {/*{{{*/
    int idx = node_t::new_space();
    node_t& node = node_t::check_new_node(idx);
    node.synt_sym = synt_sym;

    va_list argv;
    va_start(argv, argc);
    int son_nr = 0;
    for (size_t i = 0; i < argc; i++){
        int new_son = va_arg(argv, int);
        if (new_son==-1) continue;
        node.cld_idx[i] = new_son;
        son_nr++;
    }
    node.cld_nr = son_nr;
    va_end(argv);
    return idx;
}/*}}}*/
void set_root(int node_idx){ /*{{{*/
    node_t::root_idx = node_idx; 
    node_t::has_root = true; 
}/*}}}*/
void print_from_root(){/*{{{*/
    node_t::print_from(node_t::root_idx,0);
}/*}}}*/

