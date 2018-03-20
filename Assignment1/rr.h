#ifndef rr_h
#define rr_h

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

struct data;

void rr(struct data);
struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, int item);
int dequeue(struct Queue* queue);
int front(struct Queue* queue);
int rear(struct Queue* queue);
int existsInQueue(struct Queue* queue, int check);


#endif
