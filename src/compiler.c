#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "parser.tab.h"
#include "sil.h"

struct Program *prog;



int main(int argc, char *argv[]) {
    extern yydebug;
    yydebug = 1;
    prog = (struct Program *)malloc(sizeof(struct Program));
    int res = yyparse();

    if (res != 0) {
        fprintf(stderr, "Error while parsing\n");
    } else {
        printf("Parse successful\n");
    }

    return 0;
}
