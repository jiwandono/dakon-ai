CC=g++
CF=-g -O0 -Wall

default: dakon

dakon: dakon.cpp dakon.h
	${CC} ${CF} dakon.cpp -o dakon

clean:
	rm dakon
