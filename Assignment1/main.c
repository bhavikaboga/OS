#include "main.h"
#include "fcfs.h"
#include "sjf.h"
#include "rr.h"

void main()
{	
	FILE* fp = fopen("processes.in", "r");
	FILE* input = fopen("input.txt", "w");

	char c = getc(fp);
	while( c != EOF)
	{
		if(c == '#')
			while(c != '\n')
				c = getc(fp);
		fprintf(input, "%c", c);
		c = getc(fp);
	}
	
	fclose(fp);
	fclose(input);

	parser();

	if(!strcmp(data.algorithm, "fcfs"))
		fcfs(data);	

	if(!strcmp(data.algorithm, "sjf"))
		sjf(data);	

	if(!strcmp(data.algorithm, "rr"))
		rr(data);	
}

void parser()
{
	FILE* fp = fopen("input.txt", "r");
	char* buff = malloc(250);
	int i = 0, time = 0;

	while(strcmp(buff, "end") != 0)
	{
		fscanf(fp, "%s", buff);

		if(!strcmp(buff, "processcount"))
		{
			fscanf(fp, "%d", &data.processCount);
			data.processes = malloc(data.processCount * sizeof(struct process));
		}
		if(!strcmp(buff, "runfor"))
		{
			fscanf(fp, "%d", &data.runtime);
		}
		if(!strcmp(buff, "use"))
		{
			fscanf(fp, "%s", data.algorithm);
			if(!strcmp(data.algorithm, "rr"))
			{
				fscanf(fp, "%s", buff);
				if(!strcmp(buff, "quantum"))
					fscanf(fp, "%d", &data.quantum);

			}
		}

		if(!strcmp(buff, "process"))
		{
			fscanf(fp, "%s", buff);
			if(!strcmp(buff, "name"))
			{
				fscanf(fp, "%s", data.processes[i].id);
			}

			fscanf(fp, "%s", buff);
			if(!strcmp(buff, "arrival"))
			{
				fscanf(fp, "%d", &data.processes[i].arrival);
			}
			fscanf(fp, "%s", buff);
			if(!strcmp(buff, "burst"))
			{
				fscanf(fp, "%d", &data.processes[i].burst);
				time += data.processes[i].burst;
			}

			data.processes[i].wait = 0;
			data.processes[i].turnaround = 0;
			data.processes[i].finished = 0;
			i++;
		} 

	}
	fclose(fp);
}
