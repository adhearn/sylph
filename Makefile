parser: parser.y parser.l pasm.c pasm.h
	bison -vd parser.y
	flex parser.l
	cc -o $@ parser.tab.c lex.yy.c pasm.c -lfl

test: parser sample.ss
	./parser < sample.ss

clean:
	rm -f lex.yy.c
	rm -f parser.tab.*
	rm -f parser
