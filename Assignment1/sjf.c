#include "main.h"
#include "sjf.h"

void sjf(struct data data)
{
	int t, i, temp, idle, count, shortest = INT_MAX;

	FILE* fp = fopen("processes.out", "w");
	fprintf(fp, "%d processes\nUsing Shortest Job First (Pre)\n\n", data.processCount);

	// Runs for the length of the run time given
	for(t = 0; t < data.runtime; t++)
	{
		// For when a process is finished
		if(shortest != INT_MAX && data.processes[shortest].burst == 0)
		{
			fprintf(fp, "Time %d: %s finished\n", t, data.processes[shortest].id);
			data.processes[shortest].finished = t;
		}

		count = 0;
		idle = 0;
		temp = shortest;
		shortest = INT_MAX;

		for(i = 0; i < data.processCount; i++)
		{
			// Marks as each process arrives and prints the arrival time
			if(data.processes[i].arrival == t)
			{
				data.processes[i].turnaround = data.processes[i].burst;
				fprintf(fp, "Time %d: %s arrived\n", t, data.processes[i].id);
			}	
			
			// Goes through processes that have arrived
			if(data.processes[i].arrival <= t)
			{
				count++;
				// Finds process with the shortest burst time
				if(data.processes[i]. finished == 0 && (shortest == INT_MAX || (data.processes[i].burst < data.processes[shortest].burst && data.processes[i].burst > 0)))
					shortest = i;
				// Keeps track of the number of arrivered processes that are finished
				if(data.processes[i].burst == 0)
					idle++;
			} 
		}
			// Processor idle if all arrived processes completed their burst cycle
			if(count == idle)
				fprintf(fp, "Time %d: IDLE\n", t);
			// Processor selected with shortest burst time
			else if(temp != shortest)
				fprintf(fp, "Time %d: %s selected (burst %d)\n", t, data.processes[shortest].id, data.processes[shortest].burst);
		if(shortest != INT_MAX)
			data.processes[shortest].burst--;
	}

	// Checks if all processes are finished
	int fin = 1;
	for(i = 0; i < data.processCount; i++)
		if(data.processes[i].finished == 0)
			{
				fin = 0;
				break;
			}

	// Prints wait times and turnaround times if all processes finished running
	if(fin == 1)
	{
		fprintf(fp, "Finished at time %d\n\n", data.runtime);
		for(i = 0; i < data.processCount; i++)
		{
			int turnaround = 0;
			if(data.processes[i].finished != 0)
				turnaround = data.processes[i].finished - data.processes[i].arrival;
			fprintf(fp, "%s wait %d turnaround %d\n", data.processes[i].id, turnaround - data.processes[i].turnaround, turnaround);
		}
	}		
	else
		fprintf(fp, "Not finihshed at time %d\n\n", data.runtime);

	fclose(fp);
}
