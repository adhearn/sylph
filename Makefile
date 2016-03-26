CC = gcc
CFLAGS = -g -Wall
LIBS =
ifeq ($(shell uname -s),Darwin)
	LIBS += -ll
else
	LIBS += -lfl
endif

# Regular build src files
SRC_FILES=src/list.c src/compiler.c src/sil.c
GEN_SRC=parser.tab.c src/lex.yy.c
INC_DIRS=-Isrc -I.

# Test-related targets
TEST_TARGET=all_tests.out
UNITY_ROOT=../unity
TES_SRC_FILES=$(SRC_FILES) test/TestList.c $(UNITY_ROOT)/src/unity.c
TEST_INC_DIRS=$(INC_DIRS) -I$(UNITY_ROOT)/src -I$(UNITY_ROOT)/extras/fixtues/src



all: compiler

compiler: $(SRC_FILES) src/parser.y src/parser.l
	bison -vd src/parser.y
	flex -o src/lex.yy.c src/parser.l
	$(CC) $(CFLAGS) $(INC_DIRS) $(SRC_FILES) $(GEN_SRC) $(LIBS) -o $@

test: $(TEST_SRC_FILES)
	@echo -e $(TEST_SRC_FILES)
	$(CC) $(CFLAGS) $(TEST_INC_DIRS) $(TEST_SRC_FILES) -o $(TEST_TARGET)
	./$(TEST_TARGET) -v

clean:
	rm -f src/lex.yy.c
	rm -f parser.tab.*
	rm -rf parser.dSYM
	rm -f parser.output
	rm -f parser
	rm -f compiler
	rm -f all_tests.out
