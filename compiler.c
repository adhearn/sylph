#include <stdlib.h>
#include <stdio.h>
#include "parser.tab.h"
#include "pasm.h"

struct Program *prog;

int main(int argc, char *argv[]) {
  prog = (struct Program *)malloc(sizeof(struct Program));
  int res = yyparse();

  if (res != 0) {
    fprintf(stderr, "Error while parsing\n");
  }
  printf("Just after parsing, body = %p\n", prog->body);
  printf("Just for funsies, here's retval: %p\n", prog->retval);

  printf("We have a parsed program! As soon as I can print it, I will!\n");
  print_program(prog);

  return 0;
}
