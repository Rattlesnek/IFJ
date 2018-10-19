CC=gcc
CFLAGS=-std=c99 -Wall -Wextra

main: main.c token.c token.h
	$(CC) $(CFLAGS) main.c token.c -o main

scanner: scanner.c scanner.h token.c token.h
	$(CC) $(CFLAGS) scanner.c token.c -o scanner
clean:
	rm main
	rm scanner