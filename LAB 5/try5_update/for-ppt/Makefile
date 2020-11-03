# Just Type 'make' on your terminal

Mytarget1: mylibfuncns.o
	gcc main.c mylibfuncns.o -o main

mylibfuncns.o: mylibfuncns.c
	gcc mylibfuncns.c -c

clean:
	rm mylibfuncns.o main

refresh:
	rm student-data.csv
	cp student-data-copy.csv student-data.csv