extern "C" {
#include "include/AST.h"
}
#include <vector>
#include <memory>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include "include/debug.h"
static int root_idx = 0;
static bool has_root = false;
static std::vector<std::pair<bool, node_t>> node_space;

static int new_space(){/*{{{*/
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
        node_space.push_back(std::make_pair(false, empty));
    }
    return idx;
}/*}}}*/

static bool delete_space(int idx){/*{{{*/
    bool is_leaf = node_space.at(idx).second.cld_nr==0;
    bool parent_normal = false;
    if (is_leaf) {
        node_space[idx].first = false;
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

static node_t& check_new_node(int idx){/*{{{*/
    auto& node_pair = node_space.at(idx);
    Assert(node_pair.first==false, "new space return a index %d but it first is true",idx);
    node_pair.first = true;
    return node_pair.second;
}/*}}}*/

int new_leaf(synt_t synt_sym, value_t attrib) {/*{{{*/
    int idx = new_space();
    node_t& node = check_new_node(idx);
    node.cld_nr = 0;
    node.synt_sym = synt_sym;
    node.attrib = attrib;
    return idx;
}/*}}}*/

int new_node(synt_t synt_sym, int cld_nr, ...) {/*{{{*/
    int idx = new_space();
    node_t& node = check_new_node(idx);
    node.cld_nr = cld_nr;
    node.synt_sym = synt_sym;

    va_list argv;
    va_start(argv, cld_nr);
    for (size_t i = 0; i < cld_nr; i++)
        node.cld_idx[i] = va_arg(argv, int);
    va_end(argv);
    return idx;
}/*}}}*/

inline void set_root(int node_idx){ root_idx = node_idx; }
