CC = gcc
CFLAGS = -g -Wall
LIBS =
ifeq ($(shell uname -s),Darwin)
	LIBS += -ll
else
	LIBS += -lfl
endif

all: compiler

compiler: parser.y parser.l pasm.c pasm.h compiler.c
	bison -vd parser.y
	flex parser.l
	$(CC) $(CFLAGS) -o $@ parser.tab.c lex.yy.c pasm.c compiler.c $(LIBS)

test: parser sample.ss
	./parser < sample.ss

clean:
	rm -f lex.yy.c
	rm -f parser.tab.*
	rm -rf parser.dSYM
	rm -f parser.output
	rm -f parser
	rm -f compiler
