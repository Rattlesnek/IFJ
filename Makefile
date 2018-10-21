CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

all: main scanner symtable_test


main: main.c token.c token.h
	$(CC) $(CFLAGS) main.c token.c -o main

scanner: scanner.c scanner.h token.c token.h dynamicStr.c dynamicStr.h
	$(CC) $(CFLAGS) scanner.c token.c dynamicStr.c -o scanner

	#-------------------------- symtable_test---------------------------

symtable_test.o:  symtable_test.c
	gcc $(CFLAGS) -c  symtable_test.c -o  symtable_test.o 

symtable.o: symtable.h symtable.c
	gcc $(CFLAGS) -fPIC -c symtable.c -o symtable.o

symtable_test:  symtable_test.o	 symtable.o 
	gcc $(CFLAGS)  symtable_test.o symtable.o -o  symtable_test

clean:
	rm -f main
	rm -f symtable_test
	rm -f scanner
	rm -f *.o 
