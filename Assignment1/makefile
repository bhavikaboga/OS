all: main.o fcfs.o sjf.o rr.o
	gcc -o all main.o fcfs.o sjf.o rr.o

main.o: main.c fcfs.h sjf.h rr.h
	gcc -c main.c

fcfs.o: fcfs.c fcfs.h
	gcc -c fcfs.c

sjf.o: sjf.c sjf.h
	gcc -c sjf.c

rr.o: rr.c rr.h
	gcc -c rr.c

