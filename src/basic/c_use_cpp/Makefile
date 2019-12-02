CC=gcc
CPP=g++
OBJS=main.o fun.o main

main : main.o fun.o
	$(CPP) -o $@ $^

main.o : main.c
	$(CC) -c $^

fun.o : fun.cpp fun.h
	$(CPP) -c $<

clean :
	rm $(OBJS)