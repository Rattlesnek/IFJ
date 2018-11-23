CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra

all: scanner parser

parser: codeGen.c codeGen.h clean_parser main_parser.c parser.c parser.h scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h dynamicArrParam.c dynamicArrParam.h stackStr.c stackStr.h stackTkn.c stackTkn.h queue.c queue.h symtable.c symtable.h error.c error.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h parser_gen.c parser_gen.h builtin_gen.c builtin_gen.h
	gcc $(CFLAGS) codeGen.c main_parser.c parser.c error.c scanner.c token.c dynamicStr.c dynamicArrParam.c stackStr.c stackTkn.c queue.c symtable.c sa_prec.c stack_sa_prec.c parser_gen.c builtin_gen.c -o parser

dbg_parser: clean_parser main_parser.c parser.c parser.h scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h dynamicArrParam.c dynamicArrParam.h stackStr.c stackStr.h stackTkn.c stackTkn.h queue.c queue.h symtable.c symtable.h error.c error.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h funcGen.c funcGen.h
	gcc $(CFLAGS) main_parser.c parser.c error.c scanner.c token.c dynamicStr.c dynamicArrParam.c stackStr.c stackTkn.c queue.c symtable.c sa_prec.c stack_sa_prec.c funcGen.c -o parser -D PARSER_DEBUG


scanner:  main.c codeGen.c codeGen.h clean_parser  parser.c parser.h scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h dynamicArrParam.c dynamicArrParam.h stackStr.c stackStr.h stackTkn.c stackTkn.h queue.c queue.h symtable.c symtable.h error.c error.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h parser_gen.c parser_gen.h builtin_gen.c builtin_gen.h
	gcc $(CFLAGS) main.c codeGen.c parser.c error.c scanner.c token.c dynamicStr.c dynamicArrParam.c stackStr.c stackTkn.c queue.c symtable.c sa_prec.c stack_sa_prec.c parser_gen.c builtin_gen.c -o scanner

sa_prec: stackTkn.c stackTkn.h symtable.c symtable.h main_sa_prec.c scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h dynamicArrParam.c dynamicArrParam.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h
	gcc $(CFLAGS) stackTkn.c main_sa_prec.c scanner.c token.c dynamicStr.c queue.c symtable.c sa_prec.c stack_sa_prec.c dynamicArrParam.c -o sa_prec -D DEBUG_PREC

clean:
	rm -f parser
	rm -f scanner

clean_parser:
	rm -f parser