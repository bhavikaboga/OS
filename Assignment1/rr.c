#include "main.h"
#include "rr.h"

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* processes;
};

struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->processes = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->processes[queue->rear] = item;
    queue->size = queue->size + 1;
}

int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->processes[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->processes[queue->front];
}

int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->processes[queue->rear];
}
int existsInQueue(struct Queue* queue, int check)
{
    for(int x = 0; x < queue->size; x++)
    {
        if(queue->processes[x] == check)
            return 1;
    }
    return 0;
}


void rr(struct data data)
{
    struct Queue* queue = createQueue(data.processCount);
    int t, i, q = 0, temp, cp, flag;

    FILE *ofp = fopen("processes.out", "w");
    fprintf(ofp, "%d processes\nUsing Round-Robin\nQuantum %d\n\n", data.processCount, data.quantum);

    for(t = 0; t < data.runtime; t++)
    {
        flag = 0;

        for(i = 0; i < data.processCount; i++)
        {
            if(data.processes[i].arrival == t)
            {
                fprintf(ofp, "Time %d: %s arrived\n", t, data.processes[i].id);
                data.processes[i].wait = data.processes[i].burst;
            }
            //
            if(data.processes[i].arrival <= t && data.processes[i].burst > 0 && !existsInQueue(queue, i))
                enqueue(queue, i);
            if(data.processes[i].burst > 0)
                flag = 1;
        }

        cp = front(queue);

        if(data.processes[cp].burst == 0 && !isEmpty(queue))
        {
            temp = dequeue(queue);
            data.processes[temp].finished = t;
            fprintf(ofp, "Time %d: %s finished\n", t, data.processes[temp].id);
            q = 0;
            if(!isEmpty(queue))
            {
                cp = front(queue);              
                fprintf(ofp, "Time %d: %s selected (burst %d)\n", t, data.processes[cp].id,data.processes[cp].burst);
                data.processes[cp].burst--;
                q++;            
            }

        }
        else if(q < data.quantum && data.processes[cp].burst > 0 && !isEmpty(queue))
        {
            if(q == 0)
                fprintf(ofp, "Time %d: %s selected (burst %d)\n", t, data.processes[cp].id,data.processes[cp].burst);
            data.processes[cp].burst--;
            q++;
        }
        else if (q == data.quantum && !isEmpty(queue))
        {
            if(data.processes[cp].burst > 0)
            {
                temp = dequeue(queue);
                enqueue(queue, temp);
                q = 0;
                cp = front(queue);
                fprintf(ofp, "Time %d: %s selected (burst %d)\n", t, data.processes[cp].id,data.processes[cp].burst);
                data.processes[cp].burst--;
                q++;       
            }
        }
        if(flag == 0)
            fprintf(ofp, "Time %d: IDLE\n", t); 
    }

    fprintf(ofp, "Finished at time %d\n\n", t);

    for(int i = 0; i < data.processCount; i++)
    {
        int turnaround = data.processes[i].finished - data.processes[i].arrival;
        fprintf(ofp, "%s wait %d turnaround %d\n", data.processes[i].id, turnaround - data.processes[i].wait, turnaround);
    }
    
    fclose(ofp);
}
