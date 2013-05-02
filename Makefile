CC=g++
CF=-g -O2 -Wall

default: dakon

dakon: dakon.cpp dakon.h
	${CC} ${CF} dakon.cpp -o dakon

clean:
	rm dakon
