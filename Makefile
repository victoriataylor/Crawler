# myApp.makefile
#
# Guidelines for compiling crawler

CC = gcc
CFLAGS = -Wall -pedantic -std=c11
SRC= ./src/

crawler: $(SRC)crawler.c $(SRC)list.o $(SRC)hashtable.o $(SRC)web.o
	cd ./src make
	$(CC) $(CFLAGS) -o crawler $(SRC)crawler.c $(SRC)list.o $(SRC)hashtable.o $(SRC)web.o -lcurl

clean: 
	rm crawler
	cd ./src make clean
	cd ./crawledFiles && rm *

