CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra

all: parser

parser: clean main.c parser.c parser.h code_gen.c code_gen.h scanner.c scanner.h token.c token.h dynamic_str.c dynamic_str.h dynamic_arr_param.c dynamic_arr_param.h stack_str.c stack_str.h stack_tkn.c stack_tkn.h queue.c queue.h symtable.c symtable.h error.c error.h expr.c expr.h stack_expr.c stack_expr.h parser_gen.c parser_gen.h list.c list.h
	gcc $(CFLAGS) main.c parser.c code_gen.c scanner.c token.c dynamic_str.c dynamic_arr_param.c stack_str.c stack_tkn.c queue.c symtable.c error.c expr.c stack_expr.c parser_gen.c list.c -o parser

test:
	python3	./tests/main_test.py

clean:
	rm -f parser

