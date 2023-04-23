#include <cstdio>
extern "C" {
    void yyrestart (FILE *input_file);
    int yyparse (void);
    bool has_lexical_error;
    bool has_syntax_error;
}
bool has_semantic_error;
#include "ast.hh"
#include <sdt.hh>
#include <fmt/color.h>
int main(int argc, char** argv){
    if (argc <= 1) {
        fmt::print(stderr, "cc: {} no input files\ncompilation terminated.\n", 
                fmt::styled("fatal error:", fmt::fg(fmt::color::crimson)));
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        fmt::print(stderr, "cc: {} ",
                fmt::styled("fatal error:", fmt::fg(fmt::color::crimson)));
        perror(argv[1]);
        fmt::print(stderr, "compilation terminated.\n"); 
        return 1;
    }
    has_syntax_error = false;
    has_lexical_error = false;
    has_semantic_error = false;
    yyrestart(f);
    yyparse();
    if (!has_lexical_error && !has_syntax_error) {
        // print_from_root();
        auto ir = Program_c(node_t::get_node(node_t::root_idx));
        if (has_semantic_error==false)
            fmt::print("{}", ir->to_string());
    }
    return 0;
}
