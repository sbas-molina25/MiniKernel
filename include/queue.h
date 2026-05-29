#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

typedef struct nodo
{
    pcb_t proceso;
    struct nodo *siguiente;
} nodo_t;

typedef struct
{
    nodo_t *inicio;
    nodo_t *final;
    int size;
} queue_t;

void iniciarCola(queue_t *cola);

void enqueue(queue_t *cola, pcb_t proceso);

pcb_t dequeue(queue_t *cola);

int isEmpty(queue_t *cola);

#endif