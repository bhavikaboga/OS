#include "main.h"
#include "fcfs.h"

void fcfs(struct data data)
{

	int t = 0, i, j, bursts = data.processes[0].burst, wait = 0;
	FILE *fp = fopen("processes.out", "w");

	fprintf(fp, "%d processes\nUsing First Come First Served\n\n", data.processCount);
	
	// determine wait and turnaround times
	for (i = 0; i < data.processCount; i++)
	{
		data.processes[i].wait = 0;
		if (i > 0)
		{
			for (j = 0; i > 0 && j < i; j++)
				data.processes[i].wait += data.processes[j].burst;

			data.processes[i].wait -= data.processes[i].arrival;
		}

		data.processes[i].turnaround = data.processes[i].burst + data.processes[i].wait;
	}

	// execute
	while (t <= data.runtime)
	{	
		for (i = 0; i < data.processCount; i++)
		{
			if (t == data.processes[i].arrival)
				fprintf(fp, "Time %d: %s arrived\n", t, data.processes[i].id);

			if (t == data.processes[i].wait + data.processes[i].arrival)
				fprintf(fp, "Time %d: %s selected (burst %d)\n", t, data.processes[i].id, data.processes[i].burst);

			if (t == data.processes[i].turnaround + data.processes[i].arrival)
				fprintf(fp, "Time %d: %s finished\n", t, data.processes[i].id);
		}

		t++;	
	}

	fprintf(fp, "Finished at time %d\n\n", t-1);	 
	
	for ( i = 0; i < data.processCount; i++)
		fprintf(fp, "%s wait %d turnaround %d\n", data.processes[i].id, data.processes[i].wait, data.processes[i].turnaround);

	fclose(fp);

}
