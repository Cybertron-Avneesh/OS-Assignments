# Just Type 'make' on your terminal

Mytarget1: mylibfuncns.o
	gcc main.c mylibfuncns.o -o main

mylibfuncns.o: mylibfuncns.c
	gcc mylibfuncns.c -c

clean: mylibfuncns.o main
	rm mylibfuncns.o main