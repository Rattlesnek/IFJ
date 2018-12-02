CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra

all: parser

parser: clean main.c parser.c parser.h code_gen.c code_gen.h scanner.c scanner.h token.c token.h dynamic_str.c dynamic_str.h dynamic_arr_param.c dynamic_arr_param.h stack_str.c stack_str.h stack_tkn.c stack_tkn.h queue.c queue.h symtable.c symtable.h error.c error.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h parser_gen.c parser_gen.h list.c list.h
	gcc $(CFLAGS) main.c parser.c code_gen.c scanner.c token.c dynamic_str.c dynamic_arr_param.c stack_str.c stack_tkn.c queue.c symtable.c error.c sa_prec.c stack_sa_prec.c parser_gen.c list.c -o parser

dbg_parser: clean main.c parser.c parser.h code_gen.c code_gen.h scanner.c scanner.h token.c token.h dynamic_str.c dynamic_str.h dynamic_arr_param.c dynamic_arr_param.h stack_str.c stack_str.h stack_tkn.c stack_tkn.h queue.c queue.h symtable.c symtable.h error.c error.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h parser_gen.c parser_gen.h list.c list.h
	gcc $(CFLAGS) main.c parser.c code_gen.c scanner.c token.c dynamic_str.c dynamic_arr_param.c stack_str.c stack_tkn.c queue.c symtable.c error.c sa_prec.c stack_sa_prec.c parser_gen.c list.c -o parser -D PARSER_PRINT


scanner:  main_scanner.c parser.c parser.h code_gen.c code_gen.h scanner.c scanner.h token.c token.h dynamic_str.c dynamic_str.h dynamic_arr_param.c dynamic_arr_param.h stack_str.c stack_str.h stack_tkn.c stack_tkn.h queue.c queue.h symtable.c symtable.h error.c error.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h parser_gen.c parser_gen.h list.c list.h
	gcc $(CFLAGS) main_scanner.c parser.c code_gen.c scanner.c token.c dynamic_str.c dynamic_arr_param.c stack_str.c stack_tkn.c queue.c symtable.c error.c sa_prec.c stack_sa_prec.c parser_gen.c list.c -o scanner -D SCANNER_PRINT

sa_prec: main_sa_prec.c stack_tkn.c stack_tkn.h symtable.c symtable.h scanner.c scanner.h token.c token.h dynamic_str.c dynamic_str.h dynamic_arr_param.c dynamic_arr_param.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h
	gcc $(CFLAGS) main_sa_prec.c stack_tkn.c scanner.c token.c dynamic_str.c queue.c symtable.c sa_prec.c stack_sa_prec.c dynamic_arr_param.c -o sa_prec -D DEBUG_PREC

test:
	python ./tests/main_test.py


clean:
	rm -f parser

clean_all:
	rm -f parser
	rm -f scanner
	rm -f sa_prec