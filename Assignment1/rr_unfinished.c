#include <stdio.h>
#include <main.h>
#include <rr.h>
#include <stdlib.h>
#include <limits.h>

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
    printf("%d enqueued to queue\n", item);
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
    int t, i, q, temp;

    FILE *ofp = fopen("processes.out", "w");
    fprintf(fp, "%d processes\nUsing Round-Robin\nQuantum %d", data.processCount, data.quantum);

    for(t = 0; t < data.runTime; t++)
    {
        for(i = 0; i < data.processCount; i++)
        {
            if(data.processes[i].arrival == t)
                fprintf(fp, "Time %d: %s arrived\n", t, data.processes[i].id);
            //
            if(data.processes[i].arrival <= t && data.processes[i].burst > 0 && !existsInQueue(queue, i))
                enqueue(queue, i);
        }
        //process arrived and quantum begins
        if(q == 0 && !isEmpty(queue))
        {
            temp = front(queue);
            fprintf(ofp, "Time %d: %s selected (burst %d)\n", t, data.processes[temp].id,data.processes[temp].burst);
            data.processes[temp].burst--;
            if(data.processes[temp].burst == 0)
            {
                dequeue(queue);
                q = 0;
            }
            else
                q++;
        }
        //process already going
        else if(q < quantum && !isEmpty(queue))
        {
            temp = front(queue);
            data.processes[temp].burst--;
            if(data.processes[temp].burst == 0)
            {
                dequeue(queue);
                q = 0;
            }
            else
                q++;
        }
        //quantum reached, process dequeued
        else if (q == quantum && !isEmpty(queue))
        {
            temp = dequeue(queue);
            if(data.processes[temp].burst > 0)
                enqueue(queue, temp);
            q = 0;
        }
        else fprintf(ofp, "test\n");
    }
}
