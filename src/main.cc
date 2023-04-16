#include <cstdio>
extern "C" {
    void yyrestart (FILE *input_file);
    int yyparse (void);
    bool has_lexical_error;
    bool has_syntax_error;
}
#include "ast.hh"
#include <sdt.hh>
int main(int argc, char** argv){
    if (argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    has_syntax_error = false;
    has_lexical_error = false;
    yyrestart(f);
    yyparse();
    // if (!has_lexical_error && !has_syntax_error) 
    //     print_from_root();
    Program_c(node_t::get_node(node_t::root_idx));
    return 0;
}
