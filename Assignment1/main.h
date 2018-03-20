#ifndef main_h
#define main_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct data
{
	int processCount; // number of processes
	int runtime; // run time
	int quantum; // time qunatum of rr
	char algorithm[4]; // algorithm type
	struct process* processes; // holds all the processes
} data;

struct process
{
	int arrival;
	int burst;
	char id[4];
	int finished;
	int wait;
	int turnaround;
};

void parser();

#endif

