CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra

all: scanner parser

parser: main_parser.c parser.c parser.h scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h dynamicArrInt.c dynamicArrInt.h stackStr.c stackStr.h stackTkn.c stackTkn.h queue.c queue.h error.c error.h
	gcc $(CFLAGS) main_parser.c parser.c error.c scanner.c token.c dynamicStr.c dynamicArrInt.c stackStr.c stackTkn.c queue.c symtable.c -o parser

scanner: main.c scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h queue.c queue.h
	gcc $(CFLAGS)  main.c scanner.c token.c dynamicStr.c queue.c symtable.c -o scanner

sa_prec: symtable.c symtable.h main_sa_prec.c scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h dynamicArrParam.c dynamicArrParam.h sa_prec.c sa_prec.h stack_sa_prec.c stack_sa_prec.h
	gcc $(CFLAGS) main_sa_prec.c scanner.c token.c dynamicStr.c queue.c symtable.c sa_prec.c stack_sa_prec.c dynamicArrParam.c -o sa_prec
clean:
	rm -f parser
	rm -f scanner
