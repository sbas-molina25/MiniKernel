#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

typedef struct node {
    pcb_t process;
    struct node* next;
} node_t;

typedef struct {
    node_t* front;
    node_t* rear;
    int size;
} queue_t;

void init_queue(queue_t* q);

void enqueue(queue_t* q, pcb_t process);

pcb_t dequeue(queue_t* q);

int is_empty(queue_t* q);

#endif