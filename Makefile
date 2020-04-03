CC = gcc
CFLAGS = -std=c11 -Wall

mspl: $(wildcard src/*.c)
	$(CC) $(CFLAGS) -o mspl src/mspl.c

install:
	make -B
	cp ./mspl /usr/local/bin/mspl
