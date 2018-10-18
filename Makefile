CC=gcc
CFLAGS=-std=c99 -Wall -Wextra

main: main.c token.c token.h
	$(CC) $(CFLAGS) main.c token.c -o main

clean:
	rm main