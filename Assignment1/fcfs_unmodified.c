#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int number;
	int arrival;
	int burst;
	int wait; 
	int turnaround;
}process;

typedef enum {false, true} boolean;

void fcfs(int pcount, int time, process p[]);

int main()
{
	int pcount, runtime, i;
	char alg;
	scanf("%d %d %c", &pcount, &runtime, &alg);
	printf("%d processes\nUsing First Come First Served\n", pcount);
	process p[pcount];
	for (i = 0; i < pcount; i++)
		scanf("%d %d %d", &p[i].number, &p[i].arrival, &p[i].burst);
	
	fcfs(pcount, runtime, p);
	return 0;
}

void fcfs(int pcount, int time, process p[])
{
	int t = 0, i, j, bursts = p[0].burst, wait = 0;

	// determine wait and turnaround times
	for (i = 0; i < pcount; i++)
	{
		p[i].wait = 0;
		if (i > 0)
		{
			for (j = 0; i > 0 && j < i; j++)
				p[i].wait += p[j].burst;

			p[i].wait -= p[i].arrival;
		}

		p[i].turnaround = p[i].burst + p[i].wait;
	}

	// execute
	while (t <= time)
	{	
		for (i = 0; i < pcount; i++)
		{
			if (t == p[i].arrival)
				printf("Time %d: P%d arrived\n", t, p[i].number);

			if (t == p[i].wait + p[i].arrival)
				printf("Time %d: P%d selected (burst %d)\n", t, p[i].number, p[i].burst);

			if (t == p[i].turnaround + p[i].arrival)
				printf("Time %d: P%d finished\n", t, p[i].number);
		}

		t++;	
	}

	printf("Finished at time %d\n\n", t-1);	 
	
	for ( i = 0; i < pcount; i++)
		printf("P%d wait %d turnaround %d\n", p[i].number, p[i].wait, p[i].turnaround);

}


