CC = gcc
CFLAGS = -Wall -pedantic -std=c11

all: list.o hashtable.o web.o 
	
list.o: list.c list.h common.h
	$(CC) $(CFLAGS) -c list.c 

hashtable.o: hashtable.c hashtable.h common.h
	$(CC) $(CFLAGS) -c hashtable.c

web.o: web.c web.h common.h
	$(CC) $(CFLAGS) -c web.c

clean: 
	rm *.o
