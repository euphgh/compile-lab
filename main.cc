#include <cstdio>
extern "C" {
    void yyrestart (FILE *input_file);
    int yyparse (void);
#include "include/AST.h"
}
int main(int argc, char** argv){
    if (argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    print_from_root();
    return 0;
}
