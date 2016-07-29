CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3

Boggle: Boggle.c getLine.o
	${CC} ${CFLAGS} -o $@ $^ 

Boggle.o: Boggle.c getLine.c getLine.h
	${CC} ${CFLAGS} -c Boggle.c

getLine.o: getLine.h

