#include <stdio.h>
#include <stdlib.h>
#include "../include/queue.h"

void init_queue(queue_t *q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

void enqueue(queue_t *q, pcb_t process) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->process = process;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    }
    else{
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
}

pcb_t dequeue(queue_t *q) {
    pcb_t empty_process;
    empty_process.pid = -1;

    if (q->front == NULL) {
        return empty_process;
    }
    node_t *temp = q->front;
    pcb_t process = temp->process;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return process;
}

int is_empty(queue_t *q) {
    return q->front == NULL;
}